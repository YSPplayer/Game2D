/*
  创建人：YSP 
  创建日期：2025-05-19
*/
#pragma once
#include <QMovie>
#include <QList>
#include <QString>
#include <QPixmap>
#include <QKeyEvent>
#include <QMap>
#include <QLabel>
#include <QPoint>
#include <QElapsedTimer>
#include "Core/enum.h"
using namespace Game::Core;
namespace Game {
	class SharedGifFrameManager;
	enum RoleResources {
		BaShen,//八神
	};
    class ListFilter {
    public:
        template <typename T>
        static int Count(const QList<T>& list, const std::function<bool(const T&)>& filter) {
            int count = 0;
            for (const T& item : list) {
                if (filter(item)) ++count;
            }
            return count;
        }

        template <typename T>
        static bool Any(const QList<T>& list, const std::function<bool(const T&)>& filter) {
            for (const T& item : list) {
                if (filter(item)) return true;
            }
            return false;
        }

        template <typename T>
        static bool All(const QList<T>& list, const std::function<bool(const T&)>& filter) {
            for (const T& item : list) {
                if (!filter(item)) return false;
            }
            return true;
        }

        template <typename T>
        static QList<T> Where(const QList<T>& list, const std::function<bool(const T&)>& filter) {
            QList<T> result;
            for (const T& item : list) {
                if (filter(item)) result.append(item);
            }
            return result;
        }

        /// <summary>
        /// 比较函数参数一是当前最大的数，参数二是我们需要比较的数，返回true表示我们需要比较的数较大，返回false表示需要比较的数小
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="list"></param>
        /// <param name="compare"></param>
        /// <returns></returns>
        template <typename T>
        static T Max(const QList<T>& list, const std::function<bool(const T&, const T&)>& compare) {
            if (list.count() <= 0) return T();
            T result = list[0];
            for (int i = 1; i < list.count(); ++i) {
                if (compare(result, list[i]))
                    result = list[i];
            }
            return result;
        }

        /// <summary>
        /// 比较函数参数一是当前最小的数，参数二是我们需要比较的数，返回true表示我们需要比较的数较小，返回false表示需要比较的数大
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="list"></param>
        /// <param name="compare"></param>
        /// <returns></returns>
        template <typename T>
        static T Min(const QList<T>& list, const std::function<bool(const T&, const T&)>& compare) {
            if (list.count() <= 0) return T();
            T result = list[0];
            for (int i = 1; i < list.count(); ++i) {
                if (compare(result, list[i])) {
                    result = list[i];
                }
            }
            return result;
        }

    };
	class Util {
	private:
		Util() = default;
	public:
		static double deltaTime; //当前帧的时间差
		static double lastTime; //上一帧的时间
		static QMap<RoleResources, QMap<ActionType, SharedGifFrameManager*>> rolegifs;//角色帧动画
		static QMap<RoleResources, QString> rolename;
		static QMap<ActionType, QString> acitonname;
		static QMap<KeyActionType, Qt::Key> keysmap;//映射玩家键盘的行动
		static QList<QPixmap*> bgs;//游戏战斗场景
		static bool LoadResources();
		static ActionType KeyActionTypeToActionType(KeyActionType type, bool left);
		static QPixmap ExtendPixmapHeight(const QPixmap& original, qint32 targetHeight);
		static QPixmap ToMirrorPixmap(const QPixmap& original, FlipMode mode = HorizontalFlip);
		static double TruncateToTwoDecimal(double value);
		static double GetTrueSpeed(double speed);
		static QPoint FindFirstNonTransparentPixel(const QLabel* label);
		//template<typename Key, typename Value>
		//static Key GetKeyByValue(const QMap<Key, Value>& map, const Value& targetValue)
		//{
		//	typename QMap<Key, Value>::const_iterator it;
		//	for (it = map.constBegin(); it != map.constEnd(); ++it) {
		//		if (it.value() == targetValue) {
		//			return it.key();
		//		}
		//	}
		//	return Key(); // 返回默认构造的key表示未找到
		//}
	};
}