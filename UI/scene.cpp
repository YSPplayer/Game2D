/*
  创建人：YSP
  创建日期：2025-05-19
*/
#include "scene.h"
#include <QMetaMethod>
namespace Game {
	namespace UI {
		Scene::Scene(QWidget* parent):QWidget(parent) {
			resize(parent->width(), parent->height());
			container = new QWidget(this);
			container->setGeometry(0, 0, parent->width(), parent->height());
			bg = new BackGround(container);
			bg->setGeometry(0, 0, parent->width(), parent->height());
			player = new Role(bg);
			player->CreateRole(BaShen);
			player->SetAction(Default);
			setFocusPolicy(Qt::StrongFocus);//获取窗口焦点，防止需要通过点击一次窗口才能获取到焦点
		}

		void Scene::SetBackGround(const QPixmap* img) {
			bg->SetBackGround(*img);
		}

		/// <summary>
		/// 更新游戏帧
		/// </summary>
		void Scene::Update() {
			if (presseds[Util::keysmap[MoveRight]].pressed) {
				if (presseds[Util::keysmap[MoveRight]].pressedCount == 2) {
					if (player->GetForwardKeyAction() == MoveRight) {
						presseds[Util::keysmap[MoveRight]].flag = true;
						player->Move(MoveRun);
					}
				}
				else {
					player->Move(MoveRight);
				}
			}
			else if (presseds[Util::keysmap[MoveLeft]].pressed) {
				if (presseds[Util::keysmap[MoveLeft]].pressedCount == 2) {
					if (player->GetForwardKeyAction() == MoveLeft) {
						presseds[Util::keysmap[MoveLeft]].flag = true;
						player->Move(MoveRun);
					}
				}
				else {
					player->Move(MoveLeft);
				}
			}
			else if (presseds[Util::keysmap[MoveDown]].pressed) {
				player->Move(MoveDown);
			}
			else if (presseds[Util::keysmap[MoveUp]].pressed) {
				player->Move(MoveUp);
			}
			else {
				player->SetAction(ActionType::Default);
			}
			player->Update();
		}

		void Scene::Resize()
		{
		}


		void Scene::keyPressEvent(QKeyEvent* event) {
			if (event->isAutoRepeat()) {
				event->ignore();  // 忽略自动重复事件
				return;
			}

			presseds[(Qt::Key)event->key()].pressed = true;
			presseds[(Qt::Key)event->key()].pressedCount += 1;
			if (presseds[(Qt::Key)event->key()].pressedCount == 1) {
				if (!presseds[(Qt::Key)event->key()].connectTime) {
					presseds[(Qt::Key)event->key()].connectTime = true;
					doublePressTimer[(Qt::Key)event->key()] = new QTimer(this);
					//设置300毫秒触发一次
					doublePressTimer[(Qt::Key)event->key()]->setSingleShot(true);
					doublePressTimer[(Qt::Key)event->key()]->setInterval(300);

					Qt::Key key = (Qt::Key)event->key();
					connect(doublePressTimer[(Qt::Key)event->key()], &QTimer::timeout, this, [this, key]() {
						if(!presseds[key].flag) presseds[key].pressedCount = 0;
						doublePressTimer[key]->stop();
						//qDebug() << "pressedCount" << presseds[key].pressedCount;
						});
				}
				doublePressTimer[(Qt::Key)event->key()]->start();
			} 
		}

		void Scene::keyReleaseEvent(QKeyEvent* event) {
			if (event->isAutoRepeat()) {
				event->ignore();// 忽略自动重复事件
				return;
			}
			qDebug() << "Release";
			Qt::Key key = (Qt::Key)event->key();
			if (presseds[key].flag) {
				presseds[key].Reset();
				//qDebug() << "Reset";
			}
			else {
				presseds[key].pressed = false;
			}
		}


	}
}