//------------------------------------------------------------------------------------------------
modded class SCR_MapEntity: MapEntity
{
	protected SCR_PlayerController m_pFPM_PlayerController;
	protected SCR_MapMarkerManagerComponent m_pFPM_MapMarkerManager;
	protected ref map<int,ref FPM_PlayerMapMarker> m_mFPM_Markers = new map<int,ref FPM_PlayerMapMarker>();
	protected ref array<int> m_aPreviousMarkerPlayerIds = {};
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		m_pFPM_MapMarkerManager = SCR_MapMarkerManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_MapMarkerManagerComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnMapOpen(MapConfiguration config)
	{
		super.OnMapOpen(config);
		
		if (!m_pFPM_PlayerController)
			m_pFPM_PlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		
		m_pFPM_PlayerController.Rpc(m_pFPM_PlayerController.FPM_RpcAsk_Subscribe);
	}
	
	//------------------------------------------------------------------------------------------------
	void FPM_UpdateMarkerData(array<ref FPM_MarkerData> data)
	{
		// Mark all markers for deletion and then decide based on <data>, which ones to keep
		foreach(int playerId, FPM_PlayerMapMarker marker : m_mFPM_Markers)
		{
			marker.SetMarkForDeletion(true);
		}
		
		// Update or create markers
		foreach (FPM_MarkerData markerData : data)
		{
			FPM_PlayerMapMarker marker;
			
			if (m_mFPM_Markers.Find(markerData.m_iPlayerId, marker))
			{
				marker.SetData(markerData);
				marker.SetMarkForDeletion(false);
			}
			else
			{
				FPM_CreateMarker(markerData);
			};
		};
		
		// Remove all markers that are no longer needed
		foreach(int playerId, FPM_PlayerMapMarker marker : m_mFPM_Markers)
		{
			if (marker.IsMarkedForDeletion())
				FPM_RemoveMarker(playerId, marker);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnMapClose()
	{
		super.OnMapClose();
		m_pFPM_PlayerController.Rpc(m_pFPM_PlayerController.FPM_RpcAsk_Unsubscribe);
		
		foreach(int playerId, FPM_PlayerMapMarker marker : m_mFPM_Markers)
		{
			FPM_RemoveMarker(playerId, marker);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void FPM_CreateMarkers(array<ref FPM_MarkerData> data)
	{
		foreach (FPM_MarkerData markerData : data)
		{
			FPM_CreateMarker(markerData);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FPM_CreateMarker(FPM_MarkerData data)
	{
		FPM_PlayerMapMarker marker;
		
		// Skip marker creation and just apply the data if one already exists
		if (m_mFPM_Markers.Find(data.m_iPlayerId, marker))
		{
			marker.SetData(data);
			return;
		}
		
		marker = new FPM_PlayerMapMarker();
		marker.SetData(data);
		m_pFPM_MapMarkerManager.InsertLocalMarker(marker);
		marker.SetData(data);
		m_mFPM_Markers.Insert(data.m_iPlayerId, marker);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FPM_RemoveMarker(int playerId, FPM_PlayerMapMarker marker)
	{
		m_pFPM_MapMarkerManager.RemoveLocalMarker(marker);
		m_mFPM_Markers.Remove(playerId);
	}
}