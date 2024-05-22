#include <fcntl.h>
#include <stdint.h>
#include <termio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <linux/kern_levels.h>

#include <drm/drm_print.h>
#include <drm/drm_device.h>
#include <drm/drm_drv.h>
#include <drm/drm_gem.h>
#include <drm/drm_mm.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_gem_cma_helper.h>

char TTY_name[20];
int TTY_fd = -1;

static struct drm_device drm;
static struct drm_plane primary;
static struct drm_crtc crtc;
static struct drm_encoder encoder;
static struct drm_connector connector;

static const struct drm_plane_funcs vkms_plane_funcs;
static const struct drm_crtc_funcs vkms_crtc_funcs;
static const struct drm_encoder_funcs vkms_encoder_funcs;
static const struct drm_connector_funcs vkms_connector_funcs;

static struct drm_driver vkms_driver = {
    .name  = "SimpleGraphics",
    .desc  = "DRM Driver for SimpleGraphics",
    .date  = 20240520,
    .major = 1,
    .minor = 0,
    .fops = vkms_fops,
    .driver_features = DRIVER_MODESET | DRIVER_GEM,
    .dumb_create = drm_gem_cma_dumb_create,
    .gem_vm_ops  = &drm_gem_cma_vm_ops,
    .gem_free_object_unlocked = drm_gem_cma_free_object
};

static const struct file_operations vkms_fops = {
    .owner = THIS_MODULE,
    .open = drm_open,
    .release = drm_release,
    .unlocked_ioctl = drm_ioctl,
    .poll = drm_poll,
    .read = drm_read,
    .mmap = drm_gem_cma_mmap
};

static const struct drm_crtc_helper_funcs vkms_crtc_helper_funcs = {
    .dpms = vkms_crtc_dpms,
    .mode_set = vkms_crtc_mode_set,
    .prepare = vkms_crtc_prepare,
    .commit = vkms_crtc_commit,
};

static const struct drm_crtc_funcs vkms_crtc_funcs = {
    .set_config = drm_crtc_helper_set_config,
    .page_flip = vkms_crtc_page_flip,
    .destroy = drm_crtc_cleanup,
};

static const struct drm_plane_funcs vkms_plane_funcs = {
    .update_plane = drm_primary_helper_update,
    .disable_plane = drm_primary_helper_disable,
    .destroy = drm_plane_cleanup,
};

static const struct drm_mode_config_funcs vkms_mode_funcs = {
    .fb_create = drm_fb_cma_create
};

static const struct drm_connector_helper_funcs vkms_conn_helper_funcs = {
    .get_modes = vkms_connector_get_modes,
    .best_encoder = vkms_connector_best_encoder
};

static const struct drm_connector_funcs vkms_connector_funcs = {
    .dpms = drm_helper_connector_dpms,
    .fill_modes = drm_helper_probe_single_connector_modes,
    .destroy = drm_connector_cleanup
};

static const struct drm_encoder_funcs vkms_encoder_funcs = {
    .destroy = drm_encoder_cleanup
};

static const uint32_t vkms_formats[] = {
    DRM_FORMAT_ARGB8888
};

static void vkms_crtc_dpms(struct drm_crtc *crtc, int mode){}
static void vkms_crtc_prepare(struct drm_crtc *crtc){}
static void vkms_crtc_commit(struct drm_crtc *crtc){}

static int vkms_crtc_mode_set(struct drm_crtc *crtc,
  struct drm_display_mode *mode,
  struct drm_display_mode *adjusted_mode,
  int x, int y, struct drm_framebuffer *old_fb){
    if(x == 480 && y == 272) write(TTY_fd, "\x01\0", 2);
    else if(x == 640 && y == 480) write(TTY_fd, "\x01\x01", 2);
    else if(x == 800 && y == 480) write(TTY_fd, "\x01\x02", 2);
    else if(x == 800 && y == 600) write(TTY_fd, "\x01\x03", 2);
    else if(x == 1024 && y == 680) write(TTY_fd, "\x01\x04", 2);
    else if(x == 1024 && y == 768) write(TTY_fd, "\x01\x05", 2);
    else if(x == 1280 && y == 720) write(TTY_fd, "\x01\x06", 2);
    else if(x == 1920 && y == 1080) write(TTY_fd, "\x01\x07", 2);
    else return -EOPNOTSUPP;
    return 0;
}

static int vkms_crtc_page_flip(struct drm_crtc *crtc,
  struct drm_framebuffer *fb,
  struct drm_pending_vblank_event *event,
  uint32_t page_flip_flags,
  struct drm_modeset_acquire_ctx *ctx){
    unsigned long flags;
    crtc->primary->fb = fb;
    if (event){
        spin_lock_irqsave(&crtc->dev->event_lock, flags);
        drm_crtc_send_vblank_event(crtc, event);
        spin_unlock_irqrestore(&crtc->dev->event_lock, flags);
    }
    return 0;
}

static int vkms_connector_get_modes(struct drm_connector *connector){
    int count = drm_add_modes_noedid(connector, 1920, 1080);
    drm_set_preferred_mode(connector, 1024, 768);
    return count;
}

static struct drm_encoder *vkms_connector_best_encoder(struct drm_connector *connector){
    return &encoder;
}

static void vkms_modeset_init(void){
    drm_mode_config_init(&drm);
    drm.mode_config.max_width = 1920;
    drm.mode_config.max_height = 1080;
    drm.mode_config.funcs = &vkms_mode_funcs;
    drm_universal_plane_init(&drm, &primary, 0, &vkms_plane_funcs,
        vkms_formats, ARRAY_SIZE(vkms_formats),
        NULL, DRM_PLANE_TYPE_PRIMARY, NULL);
    drm_crtc_init_with_planes(&drm, &crtc, &primary, NULL, &vkms_crtc_funcs, NULL);
    drm_encoder_init(&drm, &encoder, &vkms_encoder_funcs, DRM_MODE_ENCODER_VIRTUAL, NULL);
    drm_connector_init(&drm, &connector, &vkms_connector_funcs, DRM_MODE_CONNECTOR_VIRTUAL);
}

static int __init vkms_init(void){
    /* 
        Now we will search for all TTY devices,
        And we will find the first vaild SimpleGrahpics.
    */
    struct DIR dir = opendir("/dev");
    struct dirent ptr;
    struct flock fl;
    char buf[30];
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    while(ptr = readdir(dir)){
        TTY_fd = -1;
        if(ptr.d_type == 2 && !strcmp_n(ptr.d_name, "ttyS", 4)){ // DT_CHR = 2
            if((TTY_fd = open(strcat("/dev/", ptr.d_name), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK) == -1)) continue;
            if(fcntl(TTY_fd, F_SETLK, &fl) == -1) continue;
            struct termios Opt = {0};
            tcgetattr(fd, &Opt);
            Opt.c_cflag &= ~PARENB;
            Opt.c_cflag &= ~CSTOPB;
            Opt.c_cflag &= ~CSIZE;
            Opt.c_cflag |= ~CS8;
            cfsetispeed(&Opt, B115200);
            cfsetospeed(&Opt, B115200);
            Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
            Opt.c_oflag &= ~OPOST;
            Opt.c_lflag = 0;
            Opt.c_oflag = 0;
            Opt.c_iflag = 0;
            Opt.c_cc[VINTR] = 0;        /* Ctrl-c*/
            Opt.c_cc[VQUIT] = 0;        /* Ctrl-\ */
            Opt.c_cc[VERASE] = 0;       /* del */
            Opt.c_cc[VKILL] = 0;        /* @ */
            Opt.c_cc[VEOF] = 4;         /* Ctrl-d */
            Opt.c_cc[VTIME] = 5;        /* inter-character timer, timeout VTIME*0.1 */
            Opt.c_cc[VMIN] = 0;         /* blocking read until VMIN character arrives */
            Opt.c_cc[VSWTC] = 0;        /* '\0' */
            Opt.c_cc[VSTART] = 0;       /* Ctrl-q */
            Opt.c_cc[VSTOP] = 0;        /* Ctrl-s */
            Opt.c_cc[VSUSP] = 0;        /* Ctrl-z */
            Opt.c_cc[VEOL] = 0;         /* '\0' */
            Opt.c_cc[VREPRINT] = 0;     /* Ctrl-r */
            Opt.c_cc[VDISCARD] = 0;     /* Ctrl-u */
            Opt.c_cc[VWERASE] = 0;      /* Ctrl-w */
            Opt.c_cc[VLNEXT] = 0;       /* Ctrl-v */
            Opt.c_cc[VEOL2] = 0;        /* '\0' */
            tcsetattr(fd, TCANOW, &Opt);
            if(write(TTY_fd, "\0", 1) <= 0) continue;
            udelay(100);
            if(read(TTY_fd, buf, 20) <= 0) continue;
            if(strcmp(buf, "SimpleGraphics v1.0")) continue;
            TTY_name = strcat("/dev/", ptr.d_name);
        }
    }
    closedir(dir);
    if(TTY_fd == -1){
        printk(KERN_WARNING "SimpleGraphics: /dev/ttySx %s", strerror(2));
        return -2;
    }
    printk(KERN_NOTICE "SimpleGraphics: Found device %s", TTY_name);

    drm_dev_init(&drm, &vkms_driver, NULL);
    vkms_modeset_init();
    drm_dev_register(&drm, 0);
    return 0;
}

static int __exit vkms_exit(void){
    close(TTY_fd);
    return 0;
}

MODULE_ALIAS("SimpleGraphics");
MODULE_DESCRIPTION("DRM Driver for SimpleGraphics");
MODULE_AUTHOR("L1uTongwei<1347277058@qq.com>");
MODULE_LICENSE("GPL-v3");
module_init(vkms_init);
module_exit(vkms_exit);