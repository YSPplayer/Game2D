/*
  创建人：YSP
  创建日期：2025-05-19
*/
#pragma once
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QImageReader>
#include <QVector>
#include <QImage>
#include <QTimer>
#include "resources.h"
#include "../define.h"
namespace Game {
    // 自定义GIF帧管理器类
    class SharedGifFrameManager : public QObject {
        Q_OBJECT

    public:
        SharedGifFrameManager(const QString& gifPath, QObject* parent = nullptr)
            : QObject(parent) {
            // 加载GIF并解析所有帧
            QImageReader reader(gifPath);
            frameCount = reader.imageCount();
            // 预加载所有帧
            frames.reserve(frameCount);
            //double defaultradio = Util::TruncateToTwoDecimal((double)GAME_ROLE_HEIGHT / (double)GAME_ROLE_WIDTH);
            for (int i = 0; i < frameCount; i++) {
                reader.jumpToImage(i);
                frames.append(QPixmap::fromImage(reader.read()));
               
                frameDelays.append(reader.nextImageDelay());
            };
        }

        SharedGifFrameManager(const QList<QPixmap>& maps, QObject* parent = nullptr) : QObject(parent) {
            frameCount = maps.count();
            // 预加载所有帧
            frames.reserve(frameCount);
            for (int i = 0; i < frameCount; i++) {
                frames.append(maps[i]);
                frameDelays.append(1);
            };
        }
        // 获取指定索引的帧
        QPixmap GetFrame(int index) const {
            return frames.at(index % frameCount);
        }

        // 获取帧数量
        int GetFrameCount() const {
            return frameCount;
        }

        // 获取指定帧的延迟时间(ms)
        int GetFrameDelay(int index) const {
            return frameDelays.at(index % frameCount);
        }

    private:
        QVector<QPixmap> frames;        // 存储所有帧
        QVector<int> frameDelays;      // 存储每帧延迟
        int frameCount;            // 总帧数
    };

    // 自定义动画控制器
    class CustomGifController : public QObject {
        Q_OBJECT

    public:
        CustomGifController(SharedGifFrameManager* manager, QLabel* targetLabel,bool enabletimer,bool isleft, QObject* parent = nullptr)
            : QObject(parent), isleft(isleft),frameManager(manager), label(targetLabel), currentFrame(0) {
            if (enabletimer) {
                // 创建定时器来控制帧切换
                timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, &CustomGifController::NextFrame);
            } else {
                timer = nullptr;
            }
            this->enabletimer = enabletimer;
        }
        /// <summary>
        /// 设置当前的主要的帧动画
        /// </summary>
        void SetFrame(SharedGifFrameManager* manager,bool isleft) {
            frameManager = manager;
            this->isleft = isleft;
        }
    public slots:
        void Start() {
            if (!enabletimer) return;
            if (!timer->isActive()) {
                timer->start(frameManager->GetFrameDelay(currentFrame));
            }
        }

        void Stop() {
            if (!enabletimer) return;
            timer->stop();
        }

        void SetSpeed(int percent) {
            if (!enabletimer) return;
            speedFactor = percent / 100.0;
        }

        void NextFrame() {
            // 更新到下一帧
            currentFrame = (currentFrame + 1) % frameManager->GetFrameCount();
            // 更新标签显示
            //.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);;
            const QPixmap& map = frameManager->GetFrame(currentFrame);
            label->resize(map.width() * 1.8,map.height() * 1.8);
            label->setPixmap(isleft ? map : Util::ToMirrorPixmap(map));
            //if (!enabletimer) return;
            // 设置下一帧的延迟
            //int delay = frameManager->GetFrameDelay(currentFrame); delay * 
            //timer->setInterval(speedFactor);
        }

    private:
        bool isleft;//是否为镜像
        bool enabletimer;//是否启用timer
        SharedGifFrameManager* frameManager; // 共享帧管理器
        QLabel* label;                       // 目标标签
        QTimer* timer;                       // 帧控制定时器
        int currentFrame;                    // 当前帧索引
        qreal speedFactor = 1.0;             // 速度因子
    };
}