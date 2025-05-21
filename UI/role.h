/*
  创建人：YSP
  创建日期：2025-05-19
*/
#pragma once
#include <QLabel>
#include "../Resources/gifmanage.h"
#include "../Resources/resources.h"
namespace Game {
	namespace UI {
		/// <summary>
		/// 角色类
		/// </summary>
		class Role : public QLabel{
		public:
			bool CreateRole(RoleResources type);
			void SetAction(ActionType type);
			/// <summary>
			/// 设置角色的站位
			/// </summary>
			/// <param name="isleft"></param>
			inline void SetLeftPosition(bool isleft) {
				this->isleft = isleft;
			}

			/// <summary>
			/// 返回当前玩家前进的时候需要触发的事件(因为左右的原因不一定始终是右键前进)
			/// </summary>
			/// <returns></returns>
			inline KeyActionType GetForwardKeyAction() {
				return isleft ? KeyActionType::MoveRight : KeyActionType::MoveLeft;
			}
			inline KeyActionType GetBackKeyAction() {
				return isleft ? KeyActionType::MoveLeft : KeyActionType::MoveRight;
			}
			/// <summary>
			/// 获取当前角色的站位
			/// </summary>
			/// <returns></returns>
			inline bool IsLeftPosition() {
				return isleft;
			}
			Role(QWidget* parent);
			/// <summary>
			/// 返回当前的角色速度
			/// </summary>
			/// <returns></returns>
			inline qint32 GetSpeed() {
				return speed;
			}

			/// <summary>
			/// 设置当前的角色速度
			/// </summary>
			inline void SetSpeed(qint32 speed) {
				this->speed = speed;
			}
			void Move(KeyActionType type);
			void Update();
		private:
			bool isleft;//当前角色的站位是否在左侧
			CustomGifController* gifctr;
			RoleResources roleType;
			qint32 speed;
		};
	}
}