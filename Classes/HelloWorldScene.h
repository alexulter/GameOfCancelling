#pragma once
#include "cocos2d.h"
#include "Grid.h"
#include "ui/CocosGUI.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	Grid* grid;
	cocos2d::ui::LoadingBar* scoreCounterBar;
	cocos2d::ui::Text* winText;
	cocos2d::ui::Button* restartBtn;
	void restart(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
};

