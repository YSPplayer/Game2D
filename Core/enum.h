/*
  创建人：YSP
  创建日期：2025-05-19
*/
#pragma once
namespace Game {
	namespace Core {
		enum FlipMode {
			HorizontalFlip,
			VerticalFlip,
			BothFlip
		};
		//动画种类
		enum ActionType {
			Default,
			Forward,//前进
			Back,//后退
			Run,//奔跑
			Attack,//攻击
		};
		//玩家操作种类
		enum KeyActionType {
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			MoveRun,//奔跑
			MoveAttack,//普通攻击
		};
	
	}
}
