/*
  �����ˣ�YSP
  �������ڣ�2025-05-25
*/
#pragma once
#include "role.h"
#include <QList>
//��ɫ����֡ͼ����� 
namespace Game {
	namespace UI {
		class RoleCombo :public QObject {
		public:
			RoleCombo(QObject* parent):QObject(parent) {
				count = 0;
			}
			RoleCombo(Role* role, QObject* parent):QObject(parent){
				if (role->GetRoleType() == BaShen) {
					count = 4;//��ͨһ������
					indexs = { 5,12, 22,33 };
				}
				else {
					count = 0;
				}
			};
			int count;//����������
			QList<int> indexs;//��ͨÿ�����е�ͼƬ֡����ֹλ��
		};
	}
}
