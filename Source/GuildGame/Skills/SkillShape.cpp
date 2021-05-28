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
	GridsResult->Empty();

	GridManager->GetGridsInRange(CenterIndex, GridManager->GetGridSize()*Radius, GridsResult,false);
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

	GridsResult->Empty();
	
	GridManager->GetGridsFromCenter(CenterIndex, Row*2+1, Col*2+1, GridsResult);
	return true;
}
