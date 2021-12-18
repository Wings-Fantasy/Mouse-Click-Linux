#include <gtk/gtk.h>
#include <log4cpp/Priority.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Category.hh>
#include "main.h"
#include "thread/ClickThread.h"

GtkBuilder *builder;

GtkComboBox *clickType;
GtkRadioButton *fixedInterval;
GtkRadioButton *randomInterval;
GtkSpinButton *intervalTime;
GtkComboBox *hotKey;
GtkButton *start;
GtkButton *stop;

ClickThread *clickThread;

bool isFixedDelay = true;

// 初始化日志插件
void init_log() {
    auto *osAppender = new log4cpp::OstreamAppender("osAppender", &std::cout);
    auto *pLayout = new log4cpp::PatternLayout();
    // 设置日志格式
    pLayout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S} [%p] %c: %m%n");
    osAppender->setLayout(pLayout);
    log4cpp::Category &MouseClickLog = log4cpp::Category::getInstance("MouseClick");
    log4cpp::Category &ClickThreadLog = log4cpp::Category::getInstance("ClickThread");
    MouseClickLog.addAppender(osAppender);
    ClickThreadLog.addAppender(osAppender);
    // 设置日志级别
#ifdef NDEBUG
    MouseClickLog.setPriority(log4cpp::Priority::INFO);
    ClickThreadLog.setPriority(log4cpp::Priority::INFO);
#else
    MouseClickLog.setPriority(log4cpp::Priority::DEBUG);
    ClickThreadLog.setPriority(log4cpp::Priority::DEBUG);
#endif
}

// 初始化控制器
void init_control() {
    clickType = GTK_COMBO_BOX(gtk_builder_get_object(builder, "clickType"));
    fixedInterval = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "fixedInterval"));
    randomInterval = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "randomInterval"));
    intervalTime = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "intervalTime"));
    hotKey = GTK_COMBO_BOX(gtk_builder_get_object(builder, "hotKey"));
    start = GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    stop = GTK_BUTTON(gtk_builder_get_object(builder, "stop"));

    clickThread = new ClickThread();
}

// 当选择固定间隔时，间隔时间描述文本改为“间隔时间：”
void fixedInterval_onClick() {
    gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(builder, "intervalTimeTip")), "间隔时间：");
    isFixedDelay = true;
}

// 当选择随机间隔时，间隔时间描述文本改为“最大间隔：”
void randomInterval_onClick() {
    gtk_label_set_label(GTK_LABEL(gtk_builder_get_object(builder, "intervalTimeTip")), "最大间隔：");
    isFixedDelay = false;
}

// 更改快捷键下拉框选择项后，更新注册快捷键
void hotKey_onClick() {
//    const gchar *hotKeyId = gtk_combo_box_get_active_id(hotKey);
    // TODO 注册全局热键
}

// 开始点击鼠标
void start_onClick() {
    set_control_status(FALSE);

    const gchar *clickTypeId = gtk_combo_box_get_active_id(clickType);
    gdouble intervalTimeValue = gtk_spin_button_get_value(intervalTime) * 1000;
    int delay = (int) intervalTimeValue;

    // 设置参数并启动点击线程
    clickThread->setDelayType(isFixedDelay);
    clickThread->setDelay(delay);
    clickThread->setButton((int) std::strtol(clickTypeId, nullptr, 10));
    clickThread->start();
}

// 停止点击鼠标
void stop_onClick() {
    set_control_status(TRUE);
    clickThread->stop();
}

// 设置控制器状态
void set_control_status(bool status) {
    gtk_widget_set_sensitive(reinterpret_cast<GtkWidget *>(clickType), status);
    gtk_widget_set_sensitive(reinterpret_cast<GtkWidget *>(fixedInterval), status);
    gtk_widget_set_sensitive(reinterpret_cast<GtkWidget *>(randomInterval), status);
    gtk_widget_set_sensitive(reinterpret_cast<GtkWidget *>(intervalTime), status);
//    gtk_widget_set_sensitive(reinterpret_cast<GtkWidget *>(hotKey), status);
    gtk_widget_set_sensitive(reinterpret_cast<GtkWidget *>(start), status);
    gtk_widget_set_sensitive(reinterpret_cast<GtkWidget *>(stop), !status);
}

int main(int argc,char *argv[]) {
    init_log();
    log4cpp::Category &log = log4cpp::Category::getInstance("MouseClick");
    // 固定显示名称（不知道为什么gnome桌面的顶栏左侧显示的名字会乱码）
    argv[0] = (char*)"鼠标连点器";
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_resource(builder, "/mouse_click/mouse_click.glade", nullptr);
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    init_control();

    // 注册监听事件
    g_signal_connect(G_OBJECT(fixedInterval), "clicked", G_CALLBACK(fixedInterval_onClick), NULL);
    g_signal_connect(G_OBJECT(randomInterval), "clicked", G_CALLBACK(randomInterval_onClick), NULL);
    g_signal_connect(G_OBJECT(hotKey), "changed", G_CALLBACK(hotKey_onClick), NULL);
    g_signal_connect(G_OBJECT(start), "clicked", G_CALLBACK(start_onClick), NULL);
    g_signal_connect(G_OBJECT(stop), "clicked", G_CALLBACK(stop_onClick), NULL);

    log.info("System initialized.");
    gtk_widget_show(window);
    gtk_main();
    delete clickThread;
    log.info("exit.");
    return 0;
}
