#pragma once
#include "cocos2d.h"
#include "Item.h"
#include <vector>
#include "ui/CocosGUI.h"

class Grid : public cocos2d::Node
{
public:
	CREATE_FUNC(Grid);

	bool init() override;

	void onEnter() override;

	void restart();
	void resetUI();
	void setScoreCounterBar(cocos2d::ui::LoadingBar* scoreCounterBar);
	void setWinText(cocos2d::ui::Text* winText);

protected:
	int generationCount;
	int populationCount;
	float cellWidth;
	float cellHeight;
	std::vector<cocos2d::Vector<Item*>> gridArray;
	cocos2d::Vector<Item*> allItemsArray;

	void setupGrid();
	void setupTouchHandling();
	//void updateNeighborCount();
	//void updateCreatures();
	Item* itemForTouchLocation(cocos2d::Vec2 touchLocation);
	bool isIndexInside(int row, int col);
	Item* itemForRowColumn(int row, int col);
	cocos2d::Vector<Item*> candidatesToBlwUp;
	void findCandidatesToBlowUp(int row, int col);
	cocos2d::Color3B blowColor;
	void RearrangeItemsView();

	void doWin();
	void AddProgress();
	cocos2d::ui::LoadingBar* _scoreCounterBar;
	cocos2d::ui::Text* _winText;
};	
