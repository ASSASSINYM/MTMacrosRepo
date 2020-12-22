//
//  MTFoundationMacro.h
//  iOSMacros
//
//  Created by ASSASSIN on 2020/12/22.
//

#import <Foundation/Foundation.h>

//------------------weak strong reference----------------
//--------------------弱强引用---------------------------

//判断是不是支持arc模式
#if __has_feature(objc_arc)

#define MT_WEAK_SELF          __weak __typeof(self) weak_self = self;
#define MT_WEAK(obj)          __weak __typeof(obj) weak_##obj = obj;
#define MT_STRONG_SELF        __strong __typeof(weak_self) strong_self = weak_self;
#define MT_STRONG(obj)        __strong __typeof(obj) strong_##obj = weak_##obj;

#else

#define MT_WEAK_SELF          __block __typeof(self) weak_self = self;
#define MT_WEAK(obj)          __block __typeof(obj) weak_##obj = obj;
#define MT_STRONG_SELF        __strong __typeof(weak_self) strong_self = weak_self;
#define MT_STRONG(obj)        __strong __typeof(obj) strong_##obj = weak_##obj;

#endif

//--------------------Print log---------------------------
//--------------------打印日志---------------------------

#define MT_STRING_FORMAT(fmt, ...)  [NSString stringWithFormat:(@"%z, %s(line %d) " fmt), clock(), __FUNCTION__, __LINE__, ##__VA_ARGS__]

#if DEBUG

#define MT_LOG(...)                 NSLog(__VA_ARGS__);

#define MT_LOG_FMT(fmt, ...)        NSLog((@"%s (line %d) " fmt), __FUNCTION__, __LINE__, ##__VA_ARGS__);

#define MT_LOG_IF(x, fmt, ...)      if (x) {MT_LOG_FMT(fmt, ##__VA_ARGS__);}

#define MT_PRINTF(fmt, ...)         printf(("%ld, %s (line %d) " fmt), clock(), __FUNCTION__, __LINE__, ##__VA_ARGS__);

// 打印super class
#define MT_PRINT_FATHERCLASS(obj)   [MTFoundationMacro printFatherClass:obj];

/*
 打印提供：
 时间,
 类名,
 行数,
 函数名
 */
#define MT_SUPER_LOG(...) printf("%s, %s, line:%d, %s, %s\n\n",[[MTFoundationMacro MT_stringDate] UTF8String], [[NSString stringWithFormat:@"%s", __FILE__].lastPathComponent UTF8String] ,__LINE__,__FUNCTION__, [[NSString stringWithFormat:__VA_ARGS__] UTF8String]);

#else


#define MT_LOG(...)

#define MT_LOG_FMT(fmt, ...)

#define MT_LOG_IF(x, fmt, ...)

#define MT_PRINTF(fmt, ...)

#define MT_PRINT_FATHERCLASS(obj)

#define MT_SUPER_LOG(...)


#endif


//--------------------assert---------------------------
//--------------------断言---------------------------

#if DEBUG


//当condition的值为非真值的时候，断言停留在此，并打印备注消息
//NSAssert(0, @"备注消息");
#define MT_NSAssert(condition, desc)  NSAssert(condition,desc);

#define MT_ASSERT(obj)                assert(obj)

#define MT_ASSERT_CLASS(obj, cls)     assert((obj) && MT_IS_KIND_OF(obj,cls))//断言实例有值和类型

// (assert main thread)断言在主线程中
#define MT_ASSERT_MAIN_THREAD         MT_ASSERT(MT_IS_MAIN_THREAD)


#else


#define MT_NSAssert(condition, desc)

#define MT_ASSERT(obj)

#define MT_ASSERT_CLASS(obj, cls)

#define MT_ASSERT_MAIN_THREAD


#endif


//------Code execution time(return min seconds）-------------
//------------------代码运算时间(返回毫秒时间)------------------

/*
 CGFloat time =  MT_EXECUTE_TIME(
 sleep(2);
 );
 
 MT_LOG(@"代码执行时间%fms",time);
 */
#define MT_EXECUTE_TIME(block)       [MTFoundationMacro calculateRunTimeBlock:^{block}];



//------------------Kind Of Class---------------------
//------------------类型判断---------------------------

// 判断实例类型(含父类)
#define MT_IS_KIND_OF(obj, cls)      [(obj) isKindOfClass:[cls class]]
#define MT_IS_KIND_OF_ClASSNAME(obj, classname)      [(obj) isKindOfClass:[NSClassFromString(classname) class]]

// 判断实例类型(不含父类)
#define MT_IS_MEMBER_OF(obj, cls)    [(obj) isMemberOfClass:[cls class]]

// 判断实例类型(是否是子类)
#define MT_IS_SUBCLASS_OF(obj, cls)  [(obj) isSubclassOfClass:[cls class]]


//--------------------Notification---------------------------
//--------------------通知---------------------------

#define MT_NOTI_DEFAULT                             [NSNotificationCenter defaultCenter]

// 添加观察者
#define MT_NOTI_ADD_OBSERVER(_obj,_sel,_name,_message)  [MT_NOTI_DEFAULT addObserver:_obj selector:_sel name:_name object:_message];

// 发送通知消息（同步）
#define MT_NOTI_POST(_name,_message)                  [MT_NOTI_DEFAULT postNotificationName:_name object:_message];

// 取消观察
#define MT_NOTI_REMOVE_SELF                         [MT_NOTI_DEFAULT removeObserver:self];

#define MT_NOTI_REMOVE(obj)                         [MT_NOTI_DEFAULT removeObserver:obj];

#define MT_NOTI_REMOVE_NAME(_obj,_name,_message)       [MT_NOTI_DEFAULT removeObserver:_obj name:_name object:_message];


//--------------------APP Version----------------------
//--------------------APP版本---------------------------

//获取APP版本
#ifndef MT_APP_VERSION
#define MT_APP_VERSION       [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"]

//获取APP的build版本
#define MT_APP_BUILD_VERSION [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"]

#endif


//--------------------Thread------------------------
//--------------------线程---------------------------

// (is main thread)判断是否主线程
#define MT_IS_MAIN_THREAD            [NSThread isMainThread]

// (run in main thread)使block在主线程中运行，例如下面的用法
//MT_RUN_MAIN_THREAD
//(self.view.backgroundColor = [UIColor blueColor];
// NSLog(@"打印当前线程%@",[NSThread currentThread]);
// )
#define MT_RUN_MAIN_THREAD(block)    if (MT_IS_MAIN_THREAD) {block} else {diMTatch_async(diMTatch_get_main_queue(), ^{block});}

// (run in global thread)使block在子线程中运行，例如下面的用法
//MT_RUN_GlOBAL_THREAD
//(NSLog(@"打印当前线程2%@",[NSThread currentThread]);
// )
#define MT_RUN_GlOBAL_THREAD(block)  if (!MT_IS_MAIN_THREAD) {block} else {diMTatch_async(diMTatch_get_global_queue(0,0), ^{block});}

// (safe run block)安全运行block
#define MT_BLOCK(block, ...)         if (block) {block(__VA_ARGS__);}



//--------------------SandBox Path---------------------------
//--------------------沙盒路径---------------------------

//获取沙盒主目录路径
#define MT_PATH_HOME         NSHomeDirectory()

//沙盒文档路径
/*Documents 目录：您应该将所有应用程序数据文件写入到这个目录下。这个目录用于存储用户数据或其它应该定期备份的信息。
 ②是否会被iTunes同步
 是
 */
#define MT_PATH_DOCUMENT     [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]


//沙盒library路径
/*Library目录：这个目录下有两个子目录：Preference,Caches
 ①存放内容
 苹果建议用来存放默认设置或其它状态信息。该路径下的文件夹，除Caches以外，都会被iTunes备份。
 ②是否会被iTunes同步
 是，但是要除了Caches子目录外
 */
#define MT_PATH_LIBRARY      [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex:0]


//沙盒Preference路径
/*①存放内容
 应用程序的偏好设置文件,配置目录，配置文件。我们使用NSUserDefaults写的设置数据都会保存到该目录下的一个plist文件中，这就是所谓的写到plist中！
 ②是否会被iTunes同步
 是
 */
#define MT_PATH_PREFERENCE   [[NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingPathComponent:@"Preferences"]


//沙盒Caches缓存路径
//Caches 目录：用于存放应用程序专用的支持文件，保存应用程序再次启动过程中需要的信息,存储项目缓存,常用设置。可创建子文件夹。可以用来放置您希望被备份但不希望被用户看到的数据。
/*①存放内容
 主要是缓存文件，用户使用过程中缓存都可以保存在这个目录中。前面说过，Documents目录用于保存不可再生的文件，那么这个目录就用于保存那些可再生的文件，比如网络请求的数据。鉴于此，应用程序通常还需要负责删除这些文件。
 ②是否会被iTunes同步
 否。
 */
#define MT_PATH_CACHE        [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0]

//沙盒tmp路径
//tmp 目录：这个目录用于存放临时文件，保存应用程序再次启动过程中不需要的信息。该路径下的文件不会被iTunes备份。
/*①存放内容
 各种临时文件，保存应用再次启动时不需要的文件。而且，当应用不再需要这些文件时应该主动将其删除，因为该目录下的东西随时有可能被系统清理掉，目前已知的一种可能清理的原因是系统磁盘存储空间不足的时候。
 ②是否会被iTunes同步
 否
 */
#define MT_PATH_TMP          NSTemporaryDirectory()


//--------------------file---------------------------
//--------------------文件---------------------------

//判断文件是否存在
#define MT_IS_FILE_EXIST(path)  [[NSFileManager defaultManager] fileExistsAtPath:(path)]


//--------------------local Language---------------------------
//--------------------本地语言---------------------------

//判断是否是简体中文环境,(判断前缀，后面可能有地区区分不同)
#define MT_LANGUAGE_IS_HANS  [[[NSLocale preferredLanguages] objectAtIndex:0] haMTrefix:@"zh-Hans"]

//判断是否是繁体中文环境,(判断前缀，后面可能有地区区分不同)
#define MT_LANGUAGE_IS_HANT  [[[NSLocale preferredLanguages] objectAtIndex:0] hasPrefix:@"zh-Hant"]

//判断本地语言是不是英语,(判断前缀，后面可能有地区区分不同)
#define MT_LANGUAGE_IS_EN()     [[[NSLocale preferredLanguages] objectAtIndex:0] hasPrefix:@"en"]

//多语言支持,使用系统多语言方法,省略第二个参数
#define MT_LocalizedString(key) NSLocalizedString(key, nil)     //官方多语言字符集



NS_ASSUME_NONNULL_BEGIN

@interface MTFoundationMacro : NSObject

//+(void)printBacktrace;  // 函数调用堆栈

+(void)printFatherClass:(id)obj;    // 打印super class

/**
 计算代码块的执行时间的方法,用来验证算法的执行效率等其他需要测试执行时间的代码
 
 @param block 代码块
 @return (return ms time)返回毫秒运算时间
 */
+(double)calculateRunTimeBlock:(void (^)(void))block;


/// 当前时间精确到毫秒
+ (NSString *)MT_stringDate;


@end

NS_ASSUME_NONNULL_END
