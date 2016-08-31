#include "GateBoundary.h"

GateBoundary::GateBoundary()
{
	questToCheck = nullptr;
	stageToCheck = nullptr;
}

GateBoundary::~GateBoundary()
{
}

float GateBoundary::GetOverlappingDistance()const
{
	return this->OverlappingDistance * 75.f;
}

bool GateBoundary::CheckCollision(const Vector3 &point)
{
	SetOverlappingDistance(0.f);
	SetOverlappingAxis(Vector3(0, 0, 0));
	float overlap = 9999.f;
	Vector3 overlappedAxis;
	for (int i = 0; i < VerticeNo; i++)
	{
		Projection point1 = SetProjection(this->Axis[i], this->Vertices);
		Projection point2 = SetProjectionPoint(this->Axis[i], point);
		if (!point1.DetermineCollision(point2))
			return false;
		else
		{
			float value = point1.GetOverlappingDistance(point2);
			if (overlap > value)
			{
				overlap = value;
				overlappedAxis = this->Axis[i];
			}
		}
	}
	SetOverlappingDistance(overlap);
	SetOverlappingAxis(overlappedAxis);
	if (questToCheck && stageToCheck)
	{
		if (stageToCheck->getComplete()
			&& stageToCheck->getStageNO() < questToCheck->getCurrentStage())
		{
			std::ostringstream ss;
			ss << "LOADING_" << name_;
			Scene_System::accessing().getCurrScene().onNotify(ss.str());
		}
	}
	else
	{
		std::ostringstream ss;
		ss << "LOADING_" << name_;
		Scene_System::accessing().getCurrScene().onNotify(ss.str());

	}
	return true;
}

void GateBoundary::TransitScene(const std::string &ID)
{
	Scene_System::accessing().SwitchScene(ID);
    Scene_System::accessing().getCurrScene().onNotify("TRANSITIONING");
}

void GateBoundary::SetQuestData(const std::string &name, const int &stage)
{
	questStage = stage;
	questName = name;
	InitQuest();
}

void GateBoundary::InitQuest()
{
	for (std::vector<Quest*>::iterator CurrQuest = Scene_System::accessing().QM.allQuests.begin(); CurrQuest != Scene_System::accessing().QM.allQuests.end(); ++CurrQuest)
	{
		std::string Text = (*CurrQuest)->getName();
		if (Text == questName)
		{
			questToCheck = *CurrQuest;
			break;
		}
	}
	for (std::vector<QuestStage*>::iterator CurrQuestStage = questToCheck->qStages.begin(); CurrQuestStage != questToCheck->qStages.end(); ++CurrQuestStage)
	{
		int stage = (*CurrQuestStage)->getStageNO();
		if (stage == questStage)
		{
			stageToCheck = *CurrQuestStage;
			break;
		}
	}
}