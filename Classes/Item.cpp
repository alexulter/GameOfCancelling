#include "Item.h"
using namespace cocos2d;


bool Item::init()
{
	if (!Sprite::initWithFile("bubble.png", Rect(0,0,60,60)))
	{
		return false;
	}

	//auto particleSystem = CCParticleSystemQuad::create("dissapear.plist");

	//CCParticleSystemQuad* m_emitter;// = new CCParticleSystemQuad();
	//m_emitter = CCParticleExplosion::create();
	//m_emitter->setLife(0.01);
	//m_emitter->setEmissionRate(100.0);
	////m_emitter->setDuration(0.1f);
	////m_emitter->setEndRadius(0.1f);
	//this->addChild(m_emitter);
	////m_emitter->setAnchorPoint(Vec2(0.0f, 0.0f));
	//m_emitter->setPosition(30, 30);

	return true;
}

void Item::setRowCol(int row, int col)
{
	_row = row;
	_column = col;
}

void Item::BlowUp()
{
	if (!IsAlive()) return;
	cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;

	

	actions.pushBack(cocos2d::FadeOut::create(0.1));
	
	actions.pushBack(cocos2d::CallFunc::create([=]()->void {
		Item::PlayBlowUpEffect();
	}));

	actions.pushBack(cocos2d::CallFunc::create([=]()->void {
		Item::DestroySelf();
	}));

	
	

	auto sequence = cocos2d::Sequence::create(actions);
	this->runAction(sequence);
}

void Item::PlayBlowUpEffect()
{
	particleSystem = ParticleSystemQuad::create("particle_texture.plist");
	this->getParent()->addChild(particleSystem, 10);
	particleSystem->setPosition(this->getPosition()+Vec2(30,30));
	particleSystem->setDuration(0.3);
	particleSystem->setScale(0.5);
}

void Item::DestroySelf()
{
	setVisible(false);
}

bool Item::IsAlive()
{
	//подпорка!! потом разберусь, почему null
	if (this == nullptr) return false;
	return _row != -1 && _column != -1;
}

void Item::Move(int x, int y)
{
	auto delta = (this->getPosition().y - y)/100;
	cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
	actions.pushBack(cocos2d::DelayTime::create(0.3));
	actions.pushBack(cocos2d::MoveTo::create(0.2*delta, Vec2(x, y)));
	actions.pushBack(cocos2d::MoveTo::create(0.08, Vec2(x, y+5)));
	actions.pushBack(cocos2d::MoveTo::create(0.1, Vec2(x, y)));
	auto sequence = cocos2d::Sequence::create(actions);
	this->runAction(sequence);
}

void Item::Shake()
{
	cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
//	actions.pushBack(cocos2d::DelayTime::create(0.3));
	actions.pushBack(cocos2d::MoveBy::create(0.05, Vec2(5, 0)));
	actions.pushBack(cocos2d::MoveBy::create(0.05, Vec2(-10, 0)));
	actions.pushBack(cocos2d::MoveBy::create(0.05, Vec2(10, 0)));
	actions.pushBack(cocos2d::MoveBy::create(0.05, Vec2(-5, 0)));
	auto sequence = cocos2d::Sequence::create(actions);
	this->runAction(sequence);
}