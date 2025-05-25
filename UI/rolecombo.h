/*
  创建人：YSP
  创建日期：2025-05-25
*/
#pragma once
#include "role.h"
#include <QList>
//角色连招帧图像管理 
namespace Game {
	namespace UI {
		class RoleCombo :public QObject {
		public:
			RoleCombo(QObject* parent):QObject(parent) {
				count = 0;
			}
			RoleCombo(Role* role, QObject* parent):QObject(parent){
				if (role->GetRoleType() == BaShen) {
					count = 4;//普通一共四下
					indexs = { 5,12, 22,33 };
				}
				else {
					count = 0;
				}
			};
			int count;//攻击连招数
			QList<int> indexs;//普通每次连招的图片帧数截止位置
		};
	}
}
