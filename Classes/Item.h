#pragma once
#include "cocos2d.h"
class Item : public cocos2d::Sprite
{
public:
	CREATE_FUNC(Item);

	bool init() override;
	void setRowCol(int row, int col);
	int row() { return _row; }
	int col() { return _column; }
	void BlowUp();
	bool IsAlive();
	void Move(int x, int y);
	void Shake();
protected:
	int _row;
	int _column;
	void PlayBlowUpEffect();
	void DestroySelf();
	cocos2d::ParticleSystemQuad* particleSystem;
};

