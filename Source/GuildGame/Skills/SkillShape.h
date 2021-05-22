#pragma once
#include "CoreMinimal.h"
#include "GuildGame/GridSystem/Grid.h"
#include "GuildGame/Managers/GridManager.h"

UENUM()
enum class ESkillShapeType: uint8
{
	Circle,
	Rectangle,
	Cone
};

class GUILDGAME_API SkillShape
{
public:
	virtual bool GetGridsInShape(GridManager* GridManager, int CenterIndex, TArray<Grid*>* GridsResult) = 0;
	virtual ~SkillShape() = default;
};

class GUILDGAME_API ShapeCircle : public SkillShape
{
public:
	ShapeCircle(const float Radius);
	virtual bool GetGridsInShape(GridManager* GridManager, int CenterIndex, TArray<Grid*>* GridsResult) override;
private:
	float Radius;
};

class GUILDGAME_API ShapeRectangle: public SkillShape
{
public:
	ShapeRectangle(const int Row, const int Col);
	virtual bool GetGridsInShape(GridManager* GridManager, int CenterIndex, TArray<Grid*>* GridsResult) override;
private:
	int Row;
	int Col;
};
