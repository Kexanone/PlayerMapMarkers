//------------------------------------------------------------------------------------------------
enum FPM_EIconType
{
	INFANTRY = 0,
	MOTORIZED = 1,
	MECHANIZED = 2,
	ROTARY_WING = 12
}

//------------------------------------------------------------------------------------------------
class FPM_MarkerData : Managed
{
	int m_iPlayerId = 0;
	vector m_vPosition = vector.Zero;
	string m_sDisplayName = string.Empty;
	ECharacterLifeState m_iLifeState = ECharacterLifeState.ALIVE;
	EMilitarySymbolIcon m_iIconType = FPM_EIconType.INFANTRY;
	private const int SERIALIZED_BYTES = 24;
	
	//------------------------------------------------------------------------------------------------
	static bool Extract(FPM_MarkerData instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		snapshot.SerializeInt(instance.m_iPlayerId);
		snapshot.SerializeVector(instance.m_vPosition);
		snapshot.SerializeInt(instance.m_iLifeState);
		snapshot.SerializeInt(instance.m_iIconType);
		snapshot.SerializeString(instance.m_sDisplayName);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, FPM_MarkerData instance)
	{
		snapshot.SerializeInt(instance.m_iPlayerId);
		snapshot.SerializeVector(instance.m_vPosition);
		snapshot.SerializeInt(instance.m_iLifeState);
		snapshot.SerializeInt(instance.m_iIconType);
		snapshot.SerializeString(instance.m_sDisplayName);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		snapshot.Serialize(packet, SERIALIZED_BYTES);
		snapshot.EncodeString(packet);
	}

	//------------------------------------------------------------------------------------------------
	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		snapshot.Serialize(packet, SERIALIZED_BYTES);
		snapshot.DecodeString(packet);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs , ScriptCtx ctx)
	{
		return lhs.CompareSnapshots(rhs, SERIALIZED_BYTES) && lhs.CompareStringSnapshots(rhs);
	}

	//------------------------------------------------------------------------------------------------
	static bool PropCompare(FPM_MarkerData instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		return snapshot.CompareInt(instance.m_iPlayerId)
			&& snapshot.CompareVector(instance.m_vPosition)
			&& snapshot.CompareInt(instance.m_iLifeState)
			&& snapshot.CompareInt(instance.m_iIconType)
			&& snapshot.CompareString(instance.m_sDisplayName);
	}
}