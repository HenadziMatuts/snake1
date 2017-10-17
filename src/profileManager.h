#pragma once
#include <vector>

const int MAX_PROFILE_NAME_SIZE = 15;
const int TOTAL_PROFILES = 5;

struct UserProfile {
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
	char* GetProfileName(int slot);

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

	/**
	 *
	 */
	void SwitchProfile(char *profileName);

	/**
	 *
	 */
	int OccupiedSlots();

	/**
	 *
	 */
	int CurrentProfileSlot();

private:
	UserProfile *m_CurrentProfile;

	std::vector<UserProfile> m_Profiles;
};