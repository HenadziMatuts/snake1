#pragma once

const int MAX_PROFILE_NAME_SIZE = 15;
const int TOTAL_PROFILES = 5;

struct UserProfile {
	bool m_IsActive;
	char m_ProfileName[MAX_PROFILE_NAME_SIZE];
};

class ProfileManager {
public:
	ProfileManager();

	/**
	 *
	 */
	void CreateProfile(char *profileName);

	/**
	 *
	 */
	void DeleteProfile(int slot);

	/**
	 *
	 */
	bool IsProfileActive(int slot);

	/**
	 *
	 */
	char* GetProfielName(int slot);

	/**
	 *
	 */
	char* GetCurrentProfileName();

	/**
	 *
	 */
	bool HasFreeSlot();

	/**
	 *
	 */
	void DeleteCurrentProfile();

	/**
	 *
	 */
	void NextProfile();

private:
	UserProfile *m_CurrentProfile;

	UserProfile m_Profiles[TOTAL_PROFILES];
};