/*
  创建人：YSP
  创建日期：2025-05-19
*/
#pragma once
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include "background.h"
#include "role.h"
namespace Game {
	namespace UI {
		struct PressedEvent {
			bool pressed{ false };//是否按下
			bool flag{ false };//根据flag检查是否需要停止time信号
			bool connectTime{ false };//检查当前按键是否连接了connect信号
			qint32 pressedCount{ 0 };//按下次数
			void Reset() {
				pressed = false;
				flag = false;
				pressedCount = 0;
			}
		};

		//战斗场景
		class Scene:public QWidget {
		public:
			Scene(QWidget* parent);
			void SetBackGround(const QPixmap* img);
			void Update();
		private:
			QMap<Qt::Key,QTimer*> doublePressTimer;//双次按下触发检测
			void Resize();
			QWidget* container;
			BackGround* bg;
			Role* player;
			QMap<Qt::Key, PressedEvent> presseds;//按键监听是否按下
		protected:
			void keyPressEvent(QKeyEvent* event) override;
			void keyReleaseEvent(QKeyEvent* event) override;
		};
	}
}