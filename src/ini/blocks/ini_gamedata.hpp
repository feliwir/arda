#pragma once
#include "../parser/block.hpp"

namespace arda::ini
{
	class GameData final : public Block
	{
	public:
		GameData();

		inline std::string& GetShellMapName() { return m_shellMapName; }
		inline std::string& GetMapName() { return m_mapName; }
		inline bool UseFPSLimit() { return m_useFPSLimit; }
		inline int GetFramesPerSecondLimit() { return m_framesPerSecondLimit; }
		inline bool Windowed() { return m_windowed; }
		inline int GetXResolution() { return m_XResolution; }
		inline int GetYResolution() { return m_YResolution; }
		inline int GetMaxShellScreens() { return m_maxShellScreens; }
		inline bool UseCloudMap() { return m_useCloudMap; }
		inline bool ShowObjectHealth() { return m_showObjectHealth; }
		inline bool UseShadowMapping() { return m_useShadowMapping; }
		inline double GetWaterPositionX() { return m_waterPositionX; }
		inline double GetWaterPositionY() { return m_waterPositionY; }
		inline double GetWaterPositionZ() { return m_waterPositionZ; }
		inline double GetWaterExtendX() { return m_waterExtendX; }
		inline double GetWaterExtendY() { return m_waterExtendY; }
		inline int GetWaterType() { return m_waterType; }

		inline bool DrawSkyBox() { return m_drawSkyBox; }

		inline double GetDefaultCameraMinHeight() { return m_defaultCameraMinHeight; }
		inline double GetDefaultCameraMaxHeight() { return m_defaultCameraMaxHeight; }
		inline double GetDefaultCameraPitchAngle() { return m_defaultCameraPitchAngle; }
		inline double GetDefaultCameraYawAngle() { return m_defaultCameraYawAngle; }

		inline double GetMaxCameraHeight() { return m_maxCameraHeight; }
		inline double GetMinCameraHeight() { return m_minCameraHeight; }
		inline double GetTerrainHeightAtEdgeOfMap() { return m_terrainHeightAtEdgeOfMap; }
		inline double GetGravity() { return m_gravity; }

		inline bool AudioOn() { return m_audioOn; }
		inline bool MusicOn() { return m_musicOn; }
		inline bool SoundsOn() { return m_soundsOn; }
		inline bool SpeechOn() { return m_speechOn; }
		inline bool VideoOn() { return m_videoOn; }

		// Inherited via Block
		virtual void SetProperty(std::string_view name,const Property value) override;

		// Geerbt über Block
		virtual void Register(Ini & ini, std::string_view name = "") override;
	private:
		std::string m_shellMapName;
		std::string m_mapName;
		bool m_useFPSLimit;
		int m_framesPerSecondLimit;
		bool m_windowed;
		int m_XResolution;
		int m_YResolution;
		int m_maxShellScreens;
		bool m_useCloudMap;
		bool m_showObjectHealth;
		bool m_useShadowMapping;
		double m_waterPositionX;
		double m_waterPositionY;
		double m_waterPositionZ;
		double m_waterExtendX;
		double m_waterExtendY;
		int m_waterType;

		bool m_drawSkyBox;

		double m_defaultCameraMinHeight;
		double m_defaultCameraMaxHeight;
		double m_defaultCameraPitchAngle;
		double m_defaultCameraYawAngle;
		double m_defaultCameraScrollSpeedScalar;

		double m_maxCameraHeight;
		double m_minCameraHeight;

		double m_terrainHeightAtEdgeOfMap;
		double m_gravity;

		bool m_audioOn;
		bool m_musicOn;
		bool m_soundsOn;
		bool m_speechOn;
		bool m_videoOn;
	};
}