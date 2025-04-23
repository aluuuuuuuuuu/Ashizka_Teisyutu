#include "MenuCollisionManager.h"
#include "MenuManager.h"
#include "SelectFinger.h"

MenuCollisionManager::MenuCollisionManager(MenuManager& mane):
    _manager(mane)
{
}

MenuCollisionManager::~MenuCollisionManager()
{
}

int MenuCollisionManager::Collision(Vec2 fingPos)
{
    int judge = -1;
    // すべてのボタンと判定をとる
    for (int i = 0; i < ALL_BUTTON_NUM; i++) {

        // 当たってない場合をすべて除く
        if (_manager.GetPos(i).x - _manager.GetRange(i).x > fingPos.x) {
            continue;
        }
        if (_manager.GetPos(i).x + _manager.GetRange(i).x < fingPos.x) {
            continue;
        }
        if (_manager.GetPos(i).y - _manager.GetRange(i).y > fingPos.y) {
            continue;
        }
        if (_manager.GetPos(i).y + _manager.GetRange(i).y < fingPos.y) {
            continue;
        }

        // 当たっていたら当たったボタンの定数を返す
        judge = i;
    }
    return judge;
}
