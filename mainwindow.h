/*
  创建人：YSP
  创建日期：2025-05-19
*/
#pragma once
#include <QtWidgets/QWidget>
#include "ui_mainwindow.h"
#include "define.h"
#include "Core/gameengine.h"
#include "UI/scene.h"
using namespace Game::UI;
using namespace Game::Core;
namespace Game {
    namespace Client {
        class MainWindow : public QWidget
        {
            Q_OBJECT

        public:
            MainWindow(QWidget* parent = nullptr);
            ~MainWindow();

        private:
            void GameLoop();
            Scene* pscene;
            GameEngine* engine;
            Ui::mainwindowClass ui;
            void Init();
            void BindEvent();
        };
    }
}