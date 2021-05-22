#include "SkillShape.h"
#include "GuildGame/Managers/GridManager.h"

ShapeCircle::ShapeCircle(const float Rad)
{
	Radius = Rad;
}

bool ShapeCircle::GetGridsInShape(GridManager* GridManager, int CenterIndex, TArray<Grid*>* GridsResult)
{
	if(GridManager == nullptr || GridsResult == nullptr)
	{
		return false;
	}

	GridManager->GetGridsInRange(CenterIndex,Radius,GridsResult,false);
	return true;
}

ShapeRectangle::ShapeRectangle(const int RowCount, const int ColCount)
{
	Row = RowCount;
	Col = ColCount;
}

bool ShapeRectangle::GetGridsInShape(GridManager* GridManager, int CenterIndex, TArray<Grid*>* GridsResult)
{
	if(GridManager == nullptr || GridsResult == nullptr)
	{
		return false;
	}

	GridManager->GetNeighbours(CenterIndex, Row, Col, GridsResult);
	return true;
}
