/*
  创建人：YSP
  创建日期：2025-05-19
*/
#include "role.h"
#include "../Core/enum.h"
#include "../define.h"
using namespace Game::Core;
namespace Game {
	namespace UI {
		/// <summary>
		/// 创建角色
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		bool Role::CreateRole(RoleResources type) {
			gifctr = new CustomGifController(Util::rolegifs[type][ActionType::Default],
				this, false,isleft,this);
			roleType = type;
			return true;
		}

		/// <summary>
		/// 设置人物的动作
		/// </summary>
		/// <param name="type"></param>
		void Role::SetAction(ActionType type) {
			if (gifctr == nullptr) {
				qDebug() << "[Role::SetAction]Warning:gifctr is null.";
				return;
			} 
			gifctr->SetFrame(Util::rolegifs[roleType][type], isleft);
		}

		Role::Role(QWidget* parent):QLabel(parent) {
			gifctr = nullptr;
			resize(GAME_ROLE_WIDTH, GAME_ROLE_HEIGHT);
			speed = 5;
			isleft = false;
			setScaledContents(true);//图片自动铺满整个label
		}

		/// <summary>
		/// 玩家前进或者后退的时候需要改变位置
		/// </summary>
		void Role::Move(KeyActionType type) {
	 		SetAction(Util::KeyActionTypeToActionType(type, isleft));
			if (type == KeyActionType::MoveRight) {
				move(this->x() + speed, this->y());
			}
			else if (type == KeyActionType::MoveRun) 
			{
				move(isleft ?
					this->x() + double(speed) * 1.5 :
					this->x() - double(speed) * 1.5, this->y());
			
			}
			else if (type == KeyActionType::MoveUp) {
				move(this->x(), this->y() - speed);
			}
			else if (type == KeyActionType::MoveDown) {
				move(this->x(), this->y() + speed);
			}
			else if (type == KeyActionType::MoveLeft) {
				move(this->x() - speed, this->y());
			}
		}


		/// <summary>
		/// 更新游戏帧 
		/// </summary>
		void Role::Update() {
			if (gifctr == nullptr) {
				qDebug() << "[Role::Update]Warning:gifctr is null.";
				return;
			}
			gifctr->NextFrame();
		}
	}
}