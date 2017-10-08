#include "profileManager.h"
#include "utilities.h"
#include <string.h>

ProfileManager::ProfileManager()
{
	m_CurrentProfile = nullptr;

	for (int i = 0; i < TOTAL_PROFILES; i++)
	{
		m_Profiles[i].m_IsActive = false;
		memset(m_Profiles[i].m_ProfileName, 0, MAX_PROFILE_NAME_SIZE);
	}
}

void ProfileManager::CreateProfile(char *profileName)
{
	for (int i = 0; i < TOTAL_PROFILES; i++)
	{
		if (!m_Profiles[i].m_IsActive)
		{
			m_Profiles[i].m_IsActive = true;
			strcpy_s(m_Profiles[i].m_ProfileName, profileName);

			m_CurrentProfile = &m_Profiles[i];
			return;
		}
	}

	LOG_INFO("There are no free slots left.");
}

void ProfileManager::DeleteProfile(int slot)
{
	if (slot < 0 || slot > TOTAL_PROFILES)
	{
		return;
	}

	m_Profiles[slot].m_IsActive = false;
	memset(m_Profiles[slot].m_ProfileName, 0, MAX_PROFILE_NAME_SIZE);
	if (m_CurrentProfile == &m_Profiles[slot])
	{
		m_CurrentProfile = nullptr;
	}

	for (int i = 0; i < TOTAL_PROFILES - 1; i++)
	{
		if (m_Profiles[i].m_IsActive)
		{
			if (!m_CurrentProfile)
			{
				m_CurrentProfile = &m_Profiles[i];
			}

			continue;
		}

		for (int j = i + 1; j < TOTAL_PROFILES; j++)
		{
			if (!m_Profiles[j].m_IsActive)
			{
				continue;
			}
			
			UserProfile swap = m_Profiles[j];
			m_Profiles[j] = m_Profiles[i];
			m_Profiles[i] = swap;
			break;
		}
	}
}

bool ProfileManager::IsProfileActive(int slot)
{
	if (slot < 0 || slot > TOTAL_PROFILES)
	{
		return false;
	}

	return m_Profiles[slot].m_IsActive;
}

char* ProfileManager::GetProfielName(int slot)
{
	if (slot < 0 || slot > TOTAL_PROFILES)
	{
		return nullptr;
	}

	return m_Profiles[slot].m_ProfileName;
}

char* ProfileManager::GetCurrentProfileName()
{
	return m_CurrentProfile ? m_CurrentProfile->m_ProfileName : nullptr;
}
