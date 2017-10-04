#include "ini_gamedata.hpp"
#include "../ini.hpp"

arda::ini::GameData::GameData()
{

}


void arda::ini::GameData::SetProperty(std::string_view name,const Property value)
{
	if (name == "ShellMapName")
		m_shellMapName = value.GetString();
	else if (name == "MapName")
	{
		m_mapName = value.GetString();
	}
	else if (name == "UseFPSLimit")
	{
		m_useFPSLimit = value.GetBoolean();
	}
	else if (name == "FramesPerSecondLimit")
	{
		m_framesPerSecondLimit = value.GetInteger();
	}
	else if (name == "Windowed")
	{
		m_windowed = value.GetBoolean();
	}
	else if (name == "XResolution")
	{
		m_XResolution = value.GetInteger();
	}
	else if (name == "YResolution")
	{
		m_YResolution = value.GetInteger();
	}
	else if (name == "MaxShellScreens")
	{
		m_maxShellScreens = value.GetInteger();
	}
	else if (name == "UseCloudMap")
	{
		m_useCloudMap = value.GetBoolean();
	}
	else if (name == "ShowObjectHealth")
	{
		m_showObjectHealth = value.GetBoolean();
	}
	else if (name == "UseShadowMapping")
	{
		m_useShadowMapping = value.GetBoolean();
	}
	else if (name == "WaterPositionX")
	{
		m_waterPositionX = value.GetDouble();
	}
	else if (name == "WaterPositionY")
	{
		m_waterPositionY = value.GetDouble();
	}
	else if (name == "WaterPositionZ")
	{
		m_waterPositionZ = value.GetDouble();
	}
	else if (name == "WaterExtendX")
	{
		m_waterExtendX = value.GetDouble();
	}
	else if (name == "WaterExtendY")
	{
		m_waterExtendY = value.GetDouble();
	}
	else if (name == "WaterType")
	{
		m_waterType = value.GetInteger();
	}
	else if (name == "DrawSkyBox")
	{
		m_drawSkyBox = value.GetBoolean();
	}
	else if (name == "DefaultCameraMinHeight")
	{
		m_defaultCameraMinHeight = value.GetDouble();
	}
	else if (name == "DefaultCameraMaxHeight")
	{
		m_defaultCameraMaxHeight = value.GetDouble();
	}
	else if (name == "DefaultCameraPitchAngle")
	{
		m_defaultCameraPitchAngle = value.GetDouble();
	}
	else if (name == "DefaultCameraYawAngle")
	{
		m_defaultCameraYawAngle = value.GetDouble();
	}
	else if (name == "DefaultCameraScrollSpeedScalar")
	{
		m_defaultCameraScrollSpeedScalar = value.GetDouble();
	}
	else if (name == "MaxCameraHeight")
	{
		m_maxCameraHeight = value.GetDouble();
	}
	else if (name == "MinCameraHeight")
	{
		m_minCameraHeight = value.GetDouble();
	}
	else if (name == "TerrainHeightAtEdgeOfMap")
	{
		m_terrainHeightAtEdgeOfMap = value.GetDouble();
	}
	else if (name == "Gravity")
	{
		m_gravity = value.GetDouble();
	}
	else if (name == "AudioOn")
	{
		m_audioOn = value.GetBoolean();
	}
	else if (name == "MusicOn")
	{
		m_musicOn = value.GetBoolean();
	}
	else if (name == "SoundsOn")
	{
		m_soundsOn = value.GetBoolean();
	}
	else if (name == "SpeechOn")
	{
		m_speechOn = value.GetBoolean();
	}
	else if (name == "VideoOn")
	{
		m_videoOn = value.GetBoolean();
	}
}

void arda::ini::GameData::Register(Ini & ini, std::string_view name)
{
	ini.RegisterGamedata(shared_from_this(), name);
}
