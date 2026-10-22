//
//  RXDataTransmissionService.h
//  RXDataTransmissionSDK
//
//  Created by 陈汉 on 2026/10/16.
//

#import <Foundation/Foundation.h>
#import <RXDataTransmissionSDK/RXDTInitConfig.h>
#import <RXDataTransmissionSDK/RXDTAttributionResult.h>

NS_ASSUME_NONNULL_BEGIN

/// 初始化回调。error 为 {"code": 错误码, "msg": 错误描述}。
typedef void (^RXDTRequestComplete)(NSDictionary *_Nullable response, NSDictionary *_Nullable error);
typedef void (^RXDTCompletion)(RXDTAttributionResult *result);

/**
 * RXDataTransmission 归因 SDK 入口:把 H5 页面上的业务参数送达 App。
 *
 * 两类场景:
 * - 已安装:H5 用 dtdemo://open?token=xxx 或 https://host/app/open?token=xxx 唤起 App,
 *   handleOpenURL / continueUserActivity 用 token 换参数。每次唤起都会触发。
 * - 未安装 → App Store 安装 → 首次打开:resolveInstall 把剪贴板 token 与设备指纹一起提交,
 *   交给服务端分层匹配(剪贴板 token 确定性优先,指纹概率兜底)。结果持久化,只执行一次。
 *
 * 最小接入(UIKit + Scene):
 * @code
 * // application:didFinishLaunchingWithOptions:,appid 为后台登记的 app-id
 * RXDTInitConfig *config = [[RXDTInitConfig alloc] init];
 * config.urlSchemes = @[@"dtdemo"];
 * [[RXDataTransmissionService sharedSDK] initWithAppid:@"..." config:config complete:^(NSDictionary *response, NSDictionary *error) { ... }];
 * // scene:willConnectToSession:options: / scene:openURLContexts:
 * [[RXDataTransmissionService sharedSDK] handleOpenURL:url completion:^(RXDTAttributionResult *result) { ... }];
 * // scene:willConnectToSession:options: / scene:continueUserActivity:
 * [[RXDataTransmissionService sharedSDK] continueUserActivity:userActivity completion:^(RXDTAttributionResult *result) { ... }];
 * // sceneDidBecomeActive:(首次)
 * [[RXDataTransmissionService sharedSDK] resolveInstallWithCompletion:^(RXDTAttributionResult *result) { ... }];
 * @endcode
 *
 * 所有方法都可以在任意线程调用,回调统一在主线程。
 */
@interface RXDataTransmissionService : NSObject

/// 初始化时传入的 appid,未初始化时为 nil。
@property (nonatomic, copy, readonly, nullable) NSString *appId;
/// 初始化时使用的配置,未初始化时为 nil。
@property (nonatomic, strong, readonly, nullable) RXDTInitConfig *config;

/**
 * 获取SDK实例(单例)
 */
+ (instancetype)sharedSDK;

/**
 * 使用默认配置初始化 SDK,见 initWithAppid:config:complete:
 */
- (void)initWithAppid:(NSString *)appid
             complete:(nullable RXDTRequestComplete)complete;

/**
 * 初始化 SDK
 * @param appid 后台登记的 app-id,所有请求都放在 App-Id 请求头中;
 *              1~64 位,字母或数字开头,其余为字母、数字、点、下划线或连字符
 * @param config 可选配置,传 nil 使用默认值
 * @param complete 初始化结果回调
 */
- (void)initWithAppid:(NSString *)appid
               config:(nullable RXDTInitConfig *)config
             complete:(nullable RXDTRequestComplete)complete;

///**
// * 初始化 SDK,与基础库 [RXSDK initWithConfig:complete:] 参数一致
// * @param config 初始化配置
// * @param complete 初始化结果回调
// */
//- (void)initWithConfig:(RXDTInitConfig *)config
//              complete:(nullable RXDTRequestComplete)complete;

/**
 * 启动入口:config.launchOptions / connectionOptions 中带有我方深链时用 token 换参数,
 * 否则(或深链未命中且尚未做过首启归因)执行首启归因。需在 App 进入前台后调用。
 */
- (void)launchWithCompletion:(RXDTCompletion)completion;

/**
 * 获取透传参数:最近一次新命中(深链或首启归因,不含缓存回放)且带参数的结果,
 * 持久化保存直到调用 clearTransmissionParams。
 * @return {"source": 命中通道, "click_id": 点击记录 ID, "params": {H5 业务参数}};没有待消费的参数时返回 nil
 */
- (nullable NSDictionary *)getTransmissionParams;

/**
 * 透传参数已上报或使用后调用,避免重复消费。
 */
- (void)clearTransmissionParams;

/**
 * 处理 H5 唤起 App 的 URL Scheme 或 Universal Link。
 * @return NO 表示不是我方链接(没有 token)或 SDK 未初始化,调用方应继续自己的路由逻辑。
 */
- (BOOL)handleOpenURL:(NSURL *)url completion:(RXDTCompletion)completion;

/**
 * 处理通用链接(Universal Links 通过 NSUserActivity 送达)。
 * @return NO 表示不是我方链接。
 */
- (BOOL)continueUserActivity:(NSUserActivity *)userActivity completion:(RXDTCompletion)completion;

/**
 * 从链接中取 token。接受 <scheme>://任意host?token=xxx
 * 以及 https://<universalLinkHost><universalLinkPath>?token=xxx。
 */
- (nullable NSString *)deepLinkTokenFromURL:(NSURL *)url;

/**
 * 首次启动归因。结果会持久化,之后调用直接返回缓存。
 * 需在 App 进入前台后调用(剪贴板只能在前台读取)。
 */
- (void)resolveInstallWithCompletion:(RXDTCompletion)completion;

/**
 * 首次启动归因。force 为 YES 时忽略缓存重新执行(调试用)。
 */
- (void)resolveInstallForce:(BOOL)force completion:(RXDTCompletion)completion;

/**
 * 上一次首启归因结果;尚未完成时返回 nil。
 */
- (nullable RXDTAttributionResult *)cachedInstallResult;

/**
 * 清除首启标记与深链去重记录,下次启动会重新归因(调试用)。
 */
- (void)resetInstallState;

@end

NS_ASSUME_NONNULL_END
