#include "Item.h"
#include "Grid.h"

using namespace cocos2d;

const int ROWS = 10;
const int COLUMNS = 10;
const float SCORE_PROGRESS_STEP = 1.3;


bool Grid::init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}

void Grid::onEnter()
{
	Node::onEnter();

	this->setupGrid();

	this->setupTouchHandling();
}

void Grid::restart()
{
	//regenerating grid
	for (int col = 0; col < COLUMNS; ++col)
	{
		gridArray.at(col).clear();
		for (int row = 0; row < ROWS; ++row)
		{
			Item* item = allItemsArray.at(col * 10 + row);
			item->stopAllActions();
			item->setVisible(true);
			item->setOpacity(255);

			item->setPosition(cellWidth *  float(col), cellHeight * float(row));
			item->setRowCol(row, col);
			if ((float)rand() / RAND_MAX > 0.75f) item->setColor(Color3B::MAGENTA);
			else if ((float)rand() / RAND_MAX > 0.75f) item->setColor(Color3B::YELLOW);
			else if ((float)rand() / RAND_MAX > 0.75f) item->setColor(Color3B::GREEN);
			else item->setColor(Color3B::WHITE);

			gridArray.at(col).pushBack(item);
		}
	}

}

void Grid::setupGrid()
{
	Sprite* gridSprite = this->getChildByName<Sprite*>("grid");
	cellWidth = gridSprite->getContentSize().width	/ float(COLUMNS);
	cellHeight = gridSprite->getContentSize().height / float(ROWS);

	
	gridArray.reserve(COLUMNS);

	for (int col = 0; col < COLUMNS; ++col)
	{
		Vector<Item*> v(ROWS);
		gridArray.push_back(v);
		for (int row = 0; row < ROWS; ++row)
		{
			Item* item = Item::create();

			item->setAnchorPoint(Vec2(0.0f, 0.0f));
			item->setPosition(cellWidth *  float(col), cellHeight * float(row));
			item->setRowCol(row,col);
			if ((float)rand()/RAND_MAX > 0.75f) item->setColor(Color3B::MAGENTA);
			else if ((float)rand() / RAND_MAX > 0.75f) item->setColor(Color3B::YELLOW);
			else if ((float)rand() / RAND_MAX > 0.75f) item->setColor(Color3B::GREEN); 
			else item->setColor(Color3B::WHITE);
			
			gridSprite->addChild(item);
			gridArray.at(col).pushBack(item);
			allItemsArray.pushBack(item);
		}
	}
}

void Grid::setupTouchHandling()
{
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		Sprite* gridSprite = this->getChildByName<Sprite*>("grid");

		Vec2 gridTouchLocation = gridSprite->convertTouchToNodeSpace(touch);

		Item* touchedCreature = this->itemForTouchLocation(gridTouchLocation);

		if (touchedCreature)
		{
			//code for blowing items up
			blowColor = touchedCreature->getColor();
			findCandidatesToBlowUp(touchedCreature->row(),touchedCreature->col());
			if (candidatesToBlwUp.size() > 1)
			{
				for (auto itm : candidatesToBlwUp)
				{
					itm->BlowUp();
				}
				for (auto itm : candidatesToBlwUp)
				{
					gridArray.at(itm->col()).eraseObject(itm);
					itm->setRowCol(-1, -1);
					Grid::AddProgress();
				}
				
			}
			else
			{
				// play animation for "failed to blow up"
				touchedCreature->Shake();
			}
			candidatesToBlwUp.clear();
			RearrangeItemsView();
		}

		return true;
	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

Item* Grid::itemForTouchLocation(Vec2 touchLocation)
{
	if (touchLocation.x < 0.0f || touchLocation.y < 0.0f)
	{
		return nullptr;
	}

	int row = touchLocation.y / cellHeight;
	int col = touchLocation.x / cellWidth;

	if (this->isIndexInside(row, col))
	{
		CCLOG("Touched item: (%d,%d)", row,col);
		return itemForRowColumn(row, col);
	}
	else
	{
		return nullptr;
	}
}
bool Grid::isIndexInside(int row, int col)
{
	return ((row >= 0 && row < ROWS) && (col >= 0 && col < COLUMNS));
}
Item* Grid::itemForRowColumn(int _row, int _col)
{
	if (!isIndexInside(_row,_col)) return nullptr;
	if (gridArray.size() > _col && gridArray.at(_col).size() > _row)
		return gridArray.at(_col).at(_row);
	else 
		return nullptr;
}
void Grid::findCandidatesToBlowUp(int row, int col)
{
	bool flg = false;
	for (auto itm : candidatesToBlwUp)
	{
		if (itm->row() == row && itm->col()==col)
		{
			flg = true;
			break;
		}
	}
	if (!flg && itemForRowColumn(row, col)->IsAlive())
	{
		candidatesToBlwUp.pushBack(itemForRowColumn(row, col));
		row += 1;
		if (itemForRowColumn(row, col) != nullptr && itemForRowColumn(row, col)->getColor() == blowColor) findCandidatesToBlowUp(row, col);
		row += -2;
		if (itemForRowColumn(row, col) != nullptr && itemForRowColumn(row, col)->getColor() == blowColor) findCandidatesToBlowUp(row, col);
		row += 1;
		col += 1;
		if (itemForRowColumn(row, col) != nullptr && itemForRowColumn(row, col)->getColor() == blowColor) findCandidatesToBlowUp(row, col);
		col += -2;
		if (itemForRowColumn(row, col) != nullptr && itemForRowColumn(row, col)->getColor() == blowColor) findCandidatesToBlowUp(row, col);
	}
}
void Grid::RearrangeItemsView()
{
	for (int i = 0; i < gridArray.size(); i++)
	{
		for (int j = 0; j < gridArray.at(i).size(); j++)//auto y : gridArray.at(i))
		{
			auto item = itemForRowColumn(j, i);
			if (item->row() == j && item->col() == i) continue;
			item->Move(cellWidth *  float(i), cellHeight * float(j));
			item->setRowCol(j, i);
		}
	}
}

void Grid::AddProgress()
{
	auto progress = _scoreCounterBar->getPercent();
	progress += SCORE_PROGRESS_STEP;
	if (progress >= 100)
	{
		progress = 100;
		doWin();
	}
	_scoreCounterBar->setPercent(progress);
}


void Grid::doWin()
{
	_winText->setVisible(true);
}

void Grid::resetUI()
{
	_scoreCounterBar->setPercent(0);
	_winText->setVisible(false);
}

void Grid::setScoreCounterBar(cocos2d::ui::LoadingBar * scoreCounterBar)
{
	_scoreCounterBar = scoreCounterBar;
}

void Grid::setWinText(cocos2d::ui::Text * winText)
{
	_winText = winText;
}


