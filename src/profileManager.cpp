#include "profileManager.h"
#include "utilities.h"
#include <string.h>

ProfileManager::ProfileManager()
{
	m_CurrentProfile = nullptr;
	m_Profiles.reserve(sizeof(UserProfile) * TOTAL_PROFILES);
}

void ProfileManager::CreateProfile(char *profileName)
{
	if(m_Profiles.size() >= TOTAL_PROFILES)
	{
		LOG_INFO("There are no free slots left.");
	}

	UserProfile newProfile;
	strcpy_s(newProfile.m_ProfileName, profileName);

	m_Profiles.push_back(newProfile);

	m_CurrentProfile = &m_Profiles.back();
}

void ProfileManager::DeleteProfile(int slot)
{
	if (slot < 0 || slot >= (int)m_Profiles.size())
	{
		return;
	}

	if (m_CurrentProfile == &m_Profiles[slot])
	{
		m_CurrentProfile = nullptr;
	}

	m_Profiles.erase(m_Profiles.begin() + slot);

	if (!m_CurrentProfile && m_Profiles.size())
	{
		m_CurrentProfile = (slot >= (int)m_Profiles.size()) ?
			&m_Profiles.back() : &m_Profiles[slot];
	}
}

char* ProfileManager::GetProfileName(int slot)
{
	return (slot < 0 || slot >= (int)m_Profiles.size()) ? nullptr :
		m_Profiles[slot].m_ProfileName;
}

char* ProfileManager::GetCurrentProfileName()
{
	return m_CurrentProfile ? m_CurrentProfile->m_ProfileName : nullptr;
}

bool ProfileManager::HasFreeSlot()
{
	return (TOTAL_PROFILES - m_Profiles.size()) != 0;
}

void ProfileManager::DeleteCurrentProfile()
{
	for (size_t i = 0; i < m_Profiles.size(); i++)
	{
		if (m_CurrentProfile == &m_Profiles[i])
		{
			DeleteProfile(i);
			break;
		}
	}
}

void ProfileManager::NextProfile()
{
	size_t s = m_Profiles.size();

	for (size_t i = 0; i < s; i++)
	{
		if (m_CurrentProfile == &m_Profiles[i])
		{
			uint32_t next = Utilities::ModuloSum(i, 1, s);
			m_CurrentProfile = &m_Profiles[next];

			break;
		}
	}
}

void ProfileManager::SwitchProfile(char *profileName)
{
	for (size_t i = 0; i < m_Profiles.size(); i++)
	{
		if (!strcmp(m_Profiles[i].m_ProfileName, profileName))
		{
			m_CurrentProfile = &m_Profiles[i];
			break;
		}
	}
}

int ProfileManager::OccupiedSlots()
{
	return m_Profiles.size();
}

int ProfileManager::CurrentProfileSlot()
{
	if (!m_CurrentProfile)
	{
		return -1;
	}
	else
	{
		for (size_t i = 0; i < m_Profiles.size(); i++)
		{
			if (m_CurrentProfile == &m_Profiles[i])
			{
				return i;
			}
		}
	}

	return -1;
}
