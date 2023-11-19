//------------------------------------------------------------------------------------------------
modded class SCR_BaseGameMode : BaseGameMode
{
	[Attribute(defvalue: "2", desc: "Timeout for updating marker data", category: "Player Marker")]
	protected float m_fFPM_DataUpdateTimeout;
	protected float m_fFPM_DataUpdateTimer = 0;
	
	protected ref array<ref FPM_MarkerData> m_aFPM_DataList = {};
	protected ref array<SCR_PlayerController> m_aFPM_Subscribers = {};
	
	//------------------------------------------------------------------------------------------------
	void SCR_BaseGameMode(IEntitySource src, IEntity parent)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		Activate();
		SetEventMask(EntityEvent.FRAME);
	}
	
	//------------------------------------------------------------------------------------------------
	void FPM_Subscribe(SCR_PlayerController subscriber)
	{
		m_aFPM_Subscribers.Insert(subscriber);
	}
	
	//------------------------------------------------------------------------------------------------
	void FPM_Unsubscribe(SCR_PlayerController subscriber)
	{
		m_aFPM_Subscribers.RemoveItem(subscriber);
	}
	
	//------------------------------------------------------------------------------------------------
	array<ref FPM_MarkerData> FPM_GetDataList()
	{
		return m_aFPM_DataList;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		
		m_fFPM_DataUpdateTimer += timeSlice;
		
		if (m_fFPM_DataUpdateTimer < m_fFPM_DataUpdateTimeout)
			return;
		
		m_fFPM_DataUpdateTimer = 0;
		m_aFPM_DataList.Clear();
		
		array<int> playerIds = {};
		GetGame().GetPlayerManager().GetPlayers(playerIds);
		
		foreach (int playerId : playerIds)
		{
			IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
			if (!player)
				continue;
			
			FPM_MarkerData markerData = new FPM_MarkerData();
			markerData.m_iPlayerId = playerId;
			
			IEntity vehicle = CompartmentAccessComponent.GetVehicleIn(player);
			
			if (vehicle)
			{
				SCR_BaseCompartmentManagerComponent compartmentManager = SCR_BaseCompartmentManagerComponent.Cast(vehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
				if (!compartmentManager)
					continue;
				
				array<IEntity> occupants = {};
				compartmentManager.GetOccupants(occupants);
				
				// Skip if player is not the effective commander (Currently we just go by occupant order)
				if (occupants[0] != player)
					continue;
				
				SCR_EditableVehicleComponent edit = SCR_EditableVehicleComponent.Cast(vehicle.FindComponent(SCR_EditableVehicleComponent));
				if (!edit)
					continue;
				
				int extraOccupantCount = occupants.Count() - 1;
				if (extraOccupantCount > 0)
				{
					markerData.m_sDisplayName = string.Format("%1 (%2) +%3", edit.GetDisplayName(), GetGame().GetPlayerManager().GetPlayerName(playerId), extraOccupantCount);
				}
				else
				{
					markerData.m_sDisplayName = string.Format("%1 (%2)", edit.GetDisplayName(), GetGame().GetPlayerManager().GetPlayerName(playerId));
				};
				
				markerData.m_vPosition = vehicle.GetOrigin();
				markerData.m_iIconType = FPM_EIconType.MOTORIZED;
				
				SCR_EditableEntityUIInfo entityUIInfo = SCR_EditableEntityUIInfo.Cast(edit.GetInfo());
				if (!entityUIInfo)
					continue;
				
				if (entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_APC))
				{
					markerData.m_iIconType = FPM_EIconType.MECHANIZED;		
				}
				else if (entityUIInfo.HasEntityLabel(EEditableEntityLabel.VEHICLE_HELICOPTER))
				{
					markerData.m_iIconType = FPM_EIconType.ROTARY_WING;
				};
			}
			else
			{
				markerData.m_sDisplayName = GetGame().GetPlayerManager().GetPlayerName(playerId);
				markerData.m_vPosition = player.GetOrigin();
			
				SCR_CharacterControllerComponent charCtrl = SCR_CharacterControllerComponent.Cast(player.FindComponent(SCR_CharacterControllerComponent));
				if (charCtrl)
					markerData.m_iLifeState = charCtrl.GetLifeState();
			};
			
			m_aFPM_DataList.Insert(markerData);
		};
		
		foreach (SCR_PlayerController subscriber : m_aFPM_Subscribers)
		{
			subscriber.Rpc(subscriber.FPM_RpcDo_UpdateMarkerData, m_aFPM_DataList);
		};
	}
}
