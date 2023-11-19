//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController : PlayerController
{
	protected SCR_BaseGameMode m_pFPM_GameMode;
	protected SCR_MapEntity m_pFPM_MapEntity;
	
	override protected void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!GetGame().InPlayMode())
			return;
		
		m_pFPM_GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		m_pFPM_MapEntity = SCR_MapEntity.Cast(GetGame().GetMapManager());
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void FPM_RpcAsk_Subscribe()
	{
		m_pFPM_GameMode.FPM_Subscribe(this);
		Rpc(FPM_RpcDo_Initialize, m_pFPM_GameMode.FPM_GetDataList());
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void FPM_RpcDo_Initialize(array<ref FPM_MarkerData> data)
	{
		m_pFPM_MapEntity.FPM_CreateMarkers(data);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void FPM_RpcAsk_Unsubscribe()
	{
		m_pFPM_GameMode.FPM_Unsubscribe(this);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void FPM_RpcDo_UpdateMarkerData(array<ref FPM_MarkerData> data)
	{
		m_pFPM_MapEntity.FPM_UpdateMarkerData(data);
	}
}