#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GridReader.h"

USING_NS_CC;



//using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	CSLoader* instance = CSLoader::getInstance();
	instance->registReaderObject("GridReader", (ObjectFactory::Instance) GridReader::getInstance);

    auto rootNode = CSLoader::createNode("MainScene.csb");
	Size size = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(size);
	ui::Helper::doLayout(rootNode);

	grid = rootNode->getChildByName<Grid*>("gridNode");
	scoreCounterBar = rootNode->getChildByName<cocos2d::ui::LoadingBar*>("LoadingBar_2");
	winText = rootNode->getChildByName<cocos2d::ui::Text*>("BitmapFontLabel_1");
	restartBtn = rootNode->getChildByName<cocos2d::ui::Button*>("Button_1");
	restartBtn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::restart,this));

    this->addChild(rootNode);
	
	grid->setScoreCounterBar(scoreCounterBar);
	grid->setWinText(winText);
	grid->resetUI();

    return true;
}

void HelloWorld::restart(Ref* pSender, ui::Widget::TouchEventType type)
{
	//restarting
	grid->restart();
	grid->resetUI();
}