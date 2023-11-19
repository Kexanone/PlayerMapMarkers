//------------------------------------------------------------------------------------------------
//! Player map marker class 
class FPM_PlayerMapMarker : SCR_MapMarkerBase
{
	protected bool m_bMarkedForDeletion = false;
	protected Color m_IconColor;
	protected static const ref const Color COLOR_ALIVE = Color.FromInt(0xff048be4);
	protected static const ref const Color COLOR_INCAPACITATED = Color.Red;
	protected static const ref const Color COLOR_DEAD = Color.Gray;
	
	//------------------------------------------------------------------------------------------------
	void SetMarkForDeletion(bool marked)
	{
		m_bMarkedForDeletion = marked;
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsMarkedForDeletion()
	{
		return m_bMarkedForDeletion;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetData(FPM_MarkerData data)
	{
		SetType(SCR_EMapMarkerType.PLACED_MILITARY);
		SetMarkerConfigID(data.m_iIconType);
		SetCustomText(data.m_sDisplayName);
		SetWorldPos(data.m_vPosition[0], data.m_vPosition[2]);
		
		Color color = COLOR_DEAD;
		
		switch (data.m_iLifeState)
		{
			case ECharacterLifeState.ALIVE: {color = COLOR_ALIVE; break;};
			case ECharacterLifeState.INCAPACITATED: {color = COLOR_INCAPACITATED; break;};
		};
		
		SetIconColor(color);
		
		if (m_MarkerWidgetComp)
		{
			m_MarkerWidgetComp.SetAuthor(string.Empty);
			m_MarkerWidgetComp.SetModeIconVisible(false);
		};
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetMarkerConfigID(int id)
	{
		if (m_iConfigID == id)
			return;
		
		m_iConfigID = id;
		
		if (m_MarkerWidgetComp)
			m_ConfigEntry.InitClientSettings(this, m_MarkerWidgetComp);
	}
	
	//------------------------------------------------------------------------------------------------
	override string GetCustomText()
	{
		if (m_MarkerWidgetComp)
			return m_MarkerWidgetComp.GetText();
		else
			return m_sCustomText;
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetCustomText(string text)
	{
		if (m_MarkerWidgetComp)
			m_MarkerWidgetComp.SetText(text);
		else
			m_sCustomText = text;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetIconColor(Color color)
	{
		if (m_MarkerWidgetComp)
			m_MarkerWidgetComp.SetColor(color);
		else
			m_IconColor = color;
	}
}
