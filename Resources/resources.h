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
#include "Core/enum.h"
using namespace Game::Core;
namespace Game {
	class SharedGifFrameManager;
	enum RoleResources {
		BaShen,//八神
	};
	class Util {
	private:
		Util() = default;
	public:
		static QMap<RoleResources, QMap<ActionType, SharedGifFrameManager*>> rolegifs;//角色帧动画
		static QMap<RoleResources, QString> rolename;
		static QMap<KeyActionType, Qt::Key> keysmap;//映射玩家键盘的行动
		static QList<QPixmap*> bgs;//游戏战斗场景
		static bool LoadResources();
		static ActionType KeyActionTypeToActionType(KeyActionType type, bool left);
		static QPixmap ExtendPixmapHeight(const QPixmap& original, qint32 targetHeight);
		static QPixmap ToMirrorPixmap(const QPixmap& original, FlipMode mode = HorizontalFlip);
		static double TruncateToTwoDecimal(double value);
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