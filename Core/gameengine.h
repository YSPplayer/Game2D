/*
  创建人：YSP
  创建日期：2025-05-19
*/
#pragma once
#include <QTimer>
//一般游戏60秒一帧，
namespace Game {
	namespace Core {
		class GameEngine:public QObject {
			Q_OBJECT
		public:
			GameEngine(QObject* parent):QObject(parent){
				gameTimer.setInterval(60); // 约60 FPS
				connect(&gameTimer, &QTimer::timeout, this, [this]() {
					emit update();
				});
			}
			void Start() {
				gameTimer.start();
			}
			void Stop() {
				gameTimer.stop();
			}
		private:
			QTimer gameTimer;
		signals:
			void update();
		};
	}
}