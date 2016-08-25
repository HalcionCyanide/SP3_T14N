#include "GlobalPlayer.h"
#include <fstream>
#include <sstream>
#include "../Misc/LoadEnemyData.h"

// Constructor
GlobalPlayer::GlobalPlayer()
{
	Init(0, 1, 1, false);
}

GlobalPlayer::GlobalPlayer(const int& Spell_Power, const int& CurrentHealth, const int& MaxHealth, const bool& IsInteracting)
{
	Init(Spell_Power, CurrentHealth, MaxHealth, IsInteracting);
}

// Destructor
GlobalPlayer::~GlobalPlayer()
{
	Exit();
}

// Main
void GlobalPlayer::Init(const int& Spell_Power, const int& CurrentHealth, const int& MaxHealth, const bool& IsInteracting)
{
    CurrCamera = nullptr;
	this->Spell_Power = Spell_Power;
	this->CurrentHealth = CurrentHealth;
	this->MaxHealth = MaxHealth;
	this->IsInteracting = IsInteracting;
    //LoadPlayerSave("DrivenFiles//PlayerSave1.csv");
}

void GlobalPlayer::Update(float dt)
{
	// DO STUFF
}

void GlobalPlayer::Exit()
{
	// SAVE STATS
    RewritePlayerSave("DrivenFiles//PlayerSave1.csv");
	// CLEAN UP

}

// Getters
int GlobalPlayer::GetSpellPower()
{
	return Spell_Power;
}

int GlobalPlayer::GetCurrentHealth()
{
	return CurrentHealth;
}

int GlobalPlayer::GetMaxHealth()
{
	return MaxHealth;
}

bool GlobalPlayer::GetIsInteracting()
{
	return IsInteracting;
}

// Setters
void GlobalPlayer::SetSpellPower(const int& Spell_Power)
{
	this->Spell_Power = Spell_Power;
}

void GlobalPlayer::SetCurrentHealth(const int& CurrentHealth)
{
	this->CurrentHealth = CurrentHealth;
}

void GlobalPlayer::SetMaxHealth(const int& MaxHealth)
{
	this->MaxHealth = MaxHealth;
}

void GlobalPlayer::SetIsInteracting(const bool& IsInteracting)
{
	this->IsInteracting = IsInteracting;
}

bool GlobalPlayer::LoadPlayerSave(const std::string &fileName)
{
    std::ifstream file(fileName.c_str());
    if (file.is_open())
    {
        std::string data = "";
        while (getline(file, data))
        {
            if (data == "")
                continue;
            size_t posOfComman = data.find_first_of(',');
            std::string key = data.substr(0, posOfComman);
            std::string value = data.substr(posOfComman + 1);
            convertStringToUpperCaps(key);
            if (checkWhetherTheWordInThatString("SPELLPOWER", key))
            {
                SetSpellPower(stoi(value));
            }
            else if (checkWhetherTheWordInThatString("CURRENTHEALTH", key))
            {
                SetCurrentHealth(stoi(value));
            }
            else if (checkWhetherTheWordInThatString("MAXHEALTH", key))
            {
                SetMaxHealth(stoi(value));
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool GlobalPlayer::RewritePlayerSave(const std::string &fileName)
{
    std::ifstream file(fileName.c_str());
    if (file.is_open())
    {
        std::vector<std::string> allThoseLines;
        std::string data = "";
        while (getline(file, data))
            allThoseLines.push_back(data);
        file.close();

        std::ofstream writeFile(fileName.c_str());
        for (std::vector<std::string>::iterator it = allThoseLines.begin(), end = allThoseLines.end(); it != end; ++it)
        {
            std::string thatSpecificLine = (*it);
            size_t posOfComman = thatSpecificLine.find_first_of(',');
            std::string key = (*it).substr(0, posOfComman + 1);
            convertStringToUpperCaps(thatSpecificLine);
            std::ostringstream ss;
            if (checkWhetherTheWordInThatString("SPELLPOWER", thatSpecificLine))
            {
                ss << key << Spell_Power;
                writeFile << ss.str() << std::endl;
            }
            else if (checkWhetherTheWordInThatString("CURRENTHEALTH", thatSpecificLine))
            {
                ss << key << CurrentHealth;
                writeFile << ss.str() << std::endl;
            }
            else if (checkWhetherTheWordInThatString("MAXHEALTH", thatSpecificLine))
            {
                ss << key << MaxHealth;
                writeFile << ss.str() << std::endl;
            }
            else
                writeFile << (*it) << std::endl;
        }
        writeFile.close();
        return true;
    }
    return false;
}

void GlobalPlayer::SetCurrCam(Camera3 &theCam)
{
    CurrCamera = &theCam;
}