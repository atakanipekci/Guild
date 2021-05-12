#pragma once
#include "GuildStats.generated.h"

UENUM()
enum class EFameLevels: uint8
{
	Unheard,
	Adolescent,
	Notable,
	WellKnown,
	Distinguished,
	Reputed,
	Prestigious
};


USTRUCT()
struct FGuildStats
{
	GENERATED_BODY()

	FString GuildName = "Guild Of DoubleBarrel";
	TArray<FString> FameLevelsDisplayNames = {"Unheard", "Adolescent", "Notable", "Well Known", "Distinguished", "Reputed", "Prestigious"};
	int MaxLevel = 7;
	int ExpForFirstLevel = 1000;
    int ExpForLastLevel = 1000000;
	
	int FameExperience = 0;
	int FameLevel = 1;
	EFameLevels Fame = EFameLevels::Unheard;

	explicit FGuildStats(const FGuildStats& StructForCopy)
	{
		FameExperience = StructForCopy.FameExperience;
		FameLevel = StructForCopy.FameLevel;
		Fame = StructForCopy.Fame;
	}
	FGuildStats()
	{
		
	}

	~FGuildStats()
	{
		
	}

	void GainFameExperience(int GainedExperience)
	{
		GainedExperience = FMath::Max(0, GainedExperience);
		FameExperience += GainedExperience;
		double B = log((double)ExpForLastLevel / ExpForFirstLevel) / (MaxLevel - 1);
	    double A = (double)ExpForFirstLevel / (exp(B) - 1.0);

		int SelectedLevel = 1;
		for (; SelectedLevel <= MaxLevel; ++SelectedLevel)
		{
			const int OldExp = round(A * exp(B * (SelectedLevel - 1)));
		    const int NewExp = round(A * exp(B * SelectedLevel));
		    const int CeilingExperience = NewExp - OldExp;

			if(FameExperience < CeilingExperience)
			{
				break;;
			}
		}

		FameLevel = SelectedLevel;
	    Fame = static_cast<EFameLevels>(FMath::Max(0, FameLevel - 1));
	}

	float GetLevelPercentage()
	{
		double B = log((double)ExpForLastLevel / ExpForFirstLevel) / (MaxLevel - 1);
	    double A = (double)ExpForFirstLevel / (exp(B) - 1.0);
		const int OldExp = round(A * exp(B * (FameLevel - 1)));
		const int NewExp = round(A * exp(B * FameLevel));
		
		const int CeilingExperience = NewExp - OldExp;

		
		return  static_cast<float>(FameExperience)/CeilingExperience;
	}

	 // bool operator==(const FGuildStats & ComparedData) const
  //    {
  //        return true;
  //    }
	
};
