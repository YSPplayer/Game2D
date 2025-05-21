/*
  �����ˣ�YSP
  �������ڣ�2025-05-19
*/
#pragma once
#include <QTimer>
//һ����Ϸ60��һ֡��
namespace Game {
	namespace Core {
		class GameEngine:public QObject {
			Q_OBJECT
		public:
			GameEngine(QObject* parent):QObject(parent){
				gameTimer.setInterval(60); // Լ60 FPS
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