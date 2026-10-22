//
//  RXDTInitConfig.h
//  RXDataTransmissionSDK
//
//  Created by 陈汉 on 2026/10/16.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * 初始化可选项,配合 initWithAppid:config:complete: 使用。
 */
@interface RXDTInitConfig : NSObject

#pragma mark - 与基础库 RXSdkInitConfig 一致的字段,改用 initWithAppid 后不再使用

///**
// * CP 唯一 ID，从 7 位数 1000000 开始递增
// */
//@property (nonatomic, copy) NSString *cpId;
///**
// * 瑞雪内部的应用 ID，由各 CP 自行在后台创建，字符串类型
// */
//@property (nonatomic, copy) NSString *productId;
///**
// * 瑞雪内部 CP 某应用的渠道 ID，由各 CP 自行在后台创建，字符串类型
// */
//@property (nonatomic, copy) NSString *channelId;
///**
// * 瑞雪域名地址 https://domain.com/格式
// */
//@property (nonatomic, copy) NSArray *baseUrlList;
///**
// * 首次启动是否展示用户隐私授权页面，YES展示，NO不展示
// * @note 默认NO不展示
// */
//@property (nonatomic, assign) BOOL usePrivacy;
///**
// * 自定义协议键值对
// * @note key:00001（00001为示例，实际值是瑞雪协议key）或链接  value:《用户协议》
// */
//@property (nonatomic, strong) NSMutableDictionary *agreementMap;
///**
// * 协议标题
// * @note 默认为 “用户协议和隐私政策”
// */
//@property (nonatomic, copy) NSString *agreementTitle;
///**
// * 是否打开DNS，YES打开，NO关闭，默认为NO
// */
//@property (nonatomic, assign) BOOL isUseDNS;
///**
// * 是否开启竞速，YES打开，NO关闭，默认为NO
// */
//@property (nonatomic, assign) BOOL openRacing;

#pragma mark - 基础配置

/**
 * 日志开关，true debug 模式，false release 模式，默认 debug
 */
@property (nonatomic, assign) bool isLogEnable;
/**
 * 启动参数
 */
@property (nonatomic, strong) NSDictionary *launchOptions;
/**
 * 启动参数，使用 SceneDelegate 需要传，非 SceneDelegate 不传
 */
@property (nonatomic, strong, nullable) UISceneConnectionOptions *connectionOptions API_AVAILABLE(ios(13.0));

#pragma mark - 归因相关的可选项

/// 接受的 URL Scheme,如 dtdemo(对应 dtdemo://open?token=xxx),需与 CFBundleURLSchemes 一致。
/// 默认读取 Info.plist 的 RXDTURLScheme(多个用英文逗号分隔)。
@property (nonatomic, copy) NSArray<NSString *> *urlSchemes;
/// 接受的 Universal Links 域名,链接形如 https://host/app/open?token=xxx,需与 Associated Domains 一致。
/// 默认读取 Info.plist 的 RXDTUniversalLinkHost(多个用英文逗号分隔)。
@property (nonatomic, copy) NSArray<NSString *> *universalLinkHosts;
/// Universal Links 路径前缀。默认 /app/open。
@property (nonatomic, copy) NSString *universalLinkPath;
/// 首启是否读取剪贴板 token。iOS 16+ 读取时系统会弹"允许粘贴"提示,关闭后只走指纹兜底。默认 YES。
@property (nonatomic, assign) BOOL readClipboard;
/// 命中后是否清空剪贴板中的 token,避免被重复使用或干扰用户。默认 YES。
@property (nonatomic, assign) BOOL clearClipboardAfterMatch;
/// 首启归因前等待深链的时间(秒),避免"被深链冷启动"时首启归因抢先用指纹认领同一条点击记录。默认 0.8。
@property (nonatomic, assign) NSTimeInterval deepLinkGracePeriod;
/// HTTP 超时(秒)。默认 10。
@property (nonatomic, assign) NSTimeInterval timeout;

@end

NS_ASSUME_NONNULL_END
