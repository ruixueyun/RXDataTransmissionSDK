//
//  RXDTAttributionResult.h
//  RXDataTransmissionSDK
//
//  Created by 陈汉 on 2026/10/16.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 参数的送达方式,wire name 与服务端 matchType 一致。
 * iOS 没有 APK 渠道包与 Install Referrer 的等价机制,因此只有以下几种来源。
 */
typedef NS_ENUM(NSInteger, RXDTAttributionSource) {
    /// 没有匹配到任何 H5 点击。wire name: none
    RXDTAttributionSourceNone = 0,
    /// 已安装:H5 通过 URL Scheme / Universal Links 唤起 App,链接里带 token。wire name: deeplink
    RXDTAttributionSourceDeeplink,
    /// 未安装:H5 写入剪贴板的 wf_token:xxx,App 首启读出。wire name: clipboard
    RXDTAttributionSourceClipboard,
    /// 未安装 · 兜底:设备指纹 + IP 概率匹配。wire name: probabilistic
    RXDTAttributionSourceProbabilistic,
};

FOUNDATION_EXPORT NSString *RXDTAttributionSourceWireName(RXDTAttributionSource source);
FOUNDATION_EXPORT NSString *RXDTAttributionSourceDisplayName(RXDTAttributionSource source);

/**
 * 一次归因的结果,所有回调都在主线程触发。
 *
 * 业务侧通常只关心 matched 与 data;涉及发奖等资金敏感操作时,
 * 建议只信任确定性来源(deeplink / clipboard)或 confidence 足够高的结果。
 */
@interface RXDTAttributionResult : NSObject

@property (nonatomic, assign, readonly) RXDTAttributionSource source;
@property (nonatomic, assign, readonly) BOOL matched;
/// [0,1]。确定性来源为 1.0,指纹匹配为服务端打分。
@property (nonatomic, assign, readonly) double confidence;
/// 服务端 /api/app/match 返回的完整数据:matched / matchType / confidence / clickId / matchedAt
/// 以及平铺在同一层的 H5 业务参数(渠道、邀请码、活动 ID…)。请求失败时为空。
@property (nonatomic, copy, readonly) NSDictionary<NSString *, id> *data;
/// 服务端点击记录 ID。
@property (nonatomic, copy, readonly, nullable) NSString *clickId;
/// YES 表示这是之前保存的结果,本次没有发起网络请求。
@property (nonatomic, assign, readonly) BOOL fromCache;
/// 非空表示本次归因失败(网络异常等),结果不会被缓存,下次启动会重试。
@property (nonatomic, copy, readonly, nullable) NSString *error;
/// 每个通道的执行情况,便于排查。
@property (nonatomic, copy, readonly) NSArray<NSString *> *diagnostics;
/// error 为空时为 YES。
@property (nonatomic, assign, readonly, getter=isSuccess) BOOL success;

/// 可直接 JSON 序列化的字典,便于桥接到 Unity / Cocos 等引擎层。
- (NSDictionary<NSString *, id> *)dictionaryRepresentation;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
