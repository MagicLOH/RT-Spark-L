#include "HAL.h"
#include "HAL_Def.h"
#include "HAL_Config.h"

#define DBG_TAG "HAL_SPI_Flash"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <dfs_file.h>
#include <unistd.h>
#include <fal.h>

rt_err_t HAL::SPI_Flash_Read(
    const char *partiton_name,
    const char *dest_buf,
    uint32_t start_offset,
    uint16_t font_len
)
{
    rt_err_t err = RT_EOK;

    if (!partiton_name)
    {
        LOG_E("Input param partition name is null!");
        return -1;
    }

    const struct fal_partition *partition = fal_partition_find(partiton_name);
    if (partition == RT_NULL)
    {
        LOG_E("Find partition (%s) failed!", partiton_name);
        err = -1;
        return err;
    }

    const struct fal_flash_dev *flash_dev = fal_flash_device_find(partition->flash_name);
    if (flash_dev == RT_NULL)
    {
        LOG_E("Find flash device (%s) failed!", partition->flash_name);
        err = -1;
        return err;
    }

    err = fal_partition_read(partition, start_offset, (uint8_t *)dest_buf, font_len);
    if (err < 0)
    {
        LOG_E("read flash device (%s) failed!", partition->flash_name);
        return err;
    }

    return RT_EOK;
}

#define BUF_SIZE 512
rt_err_t HAL::SPI_Flash_FontLibUpdate(
    const char *partiton_name,
    const char *lib_path
)
{
    rt_err_t ret = 0;
    uint32_t i, j, len;
    const struct fal_flash_dev *flash_dev = RT_NULL;
    const struct fal_partition *partition = RT_NULL;

    /*!< start to update chinese font libraries */
    int fd = open(lib_path, O_RDONLY);
    if (fd < 0)
    {
        LOG_E("open fill failed, please check path is exist!");
        ret = -RT_EINVAL;
        return ret;
    }
    struct stat f_stat = {0};
    fstat(fd, &f_stat);
    off_t FontLib_size = f_stat.st_size;
    LOG_D("FontLib_size=%d(%dK)", FontLib_size, FontLib_size / 1024);

//    const uint16_t buf_size = 1024 * 4;
//    char *buf = (char *)rt_malloc(buf_size);
//    RT_ASSERT(buf);

    // 1. erase whole partition
    if (!partiton_name)
    {
        LOG_E("Input param partition name is null!");
        return -1;
    }

    partition = fal_partition_find(partiton_name);
    if (partition == RT_NULL)
    {
        LOG_E("Find partition (%s) failed!", partiton_name);
        ret = -1;
        return ret;
    }

    flash_dev = fal_flash_device_find(partition->flash_name);
    if (flash_dev == RT_NULL)
    {
        LOG_E("Find flash device (%s) failed!", partition->flash_name);
        ret = -1;
        return ret;
    }

    LOG_I("Flash device : %s   "
          "Flash size : %dK   "
          "Partition : %s   "
          "Partition size: %dK",
          partition->flash_name,
          flash_dev->len / 1024,
          partition->name,
          partition->len / 1024);

    /* 擦除 `partition` 分区上的全部数据 */
    ret = fal_partition_erase_all(partition);
    if (ret < 0)
    {
        LOG_E("Partition (%s) erase failed!", partition->name);
        ret = -1;
        return ret;
    }
    LOG_I("Erase (%s) partition finish!", partiton_name);

    char buf[BUF_SIZE] = {0};
    /* 循环读取整个分区的数据，并对内容进行检验 */
    for (i = 0; i < partition->len;)
    {
        rt_memset(buf, 0x00, BUF_SIZE);
        len = (partition->len - i) > BUF_SIZE ? BUF_SIZE : (partition->len - i);

        /* 从 Flash 读取 len 长度的数据到 buf 缓冲区 */
        ret = fal_partition_read(partition, i, (uint8_t *)buf, len);
        if (ret < 0)
        {
            LOG_E("Partition (%s) read failed!", partition->name);
            ret = -1;
            return ret;
        }
        for (j = 0; j < len; j++)
        {
            /* 校验数据内容是否为 0xFF */
            if (buf[j] != 0xFF)
            {
                LOG_E("The erase operation did not really succeed!");
                ret = -2;
                return ret;
            }
        }
        i += len;
    }

    // 2. start update lib
    ssize_t rx = 0;
    uint32_t total = 0;
    rt_memset(buf, 0x00, BUF_SIZE);
    while (total < FontLib_size && total < partition->len)
    {
        rx = read(fd, buf, sizeof(buf));
//        rt_kprintf("read size = %d\n", s);
        if (rx < 0)
        {
            LOG_E("read font lib failed!");
            break;
        }

//        for (int k = 0; k < BUF_SIZE; k++)
//            rt_kprintf("0x%02X ", buf[k]);

        ret = fal_partition_write(partition, total, (uint8_t *)buf, rx);
        if (ret < 0)
        {
            LOG_E("Partition (%s) write failed!", partition->name);
            ret = -3;
            return ret;
        }
        total += rx;
    }

    LOG_I("Write (%s) partition finish! Write size %d(%dK).", partiton_name, total, total / 1024);

    close(fd);
    return ret;
}

void HAL::SPI_Flash_MountFS()
{
#ifdef BSP_USING_FLASH_FATFS
#define FS_PARTITION_NAME "filesystem"

    extern int fal_init(void);
    extern struct rt_device *fal_blk_device_create(const char *parition_name);
    fal_init();
    /* 在 spi flash 中名为 "filesystem" 的分区上创建一个块设备 */
    struct rt_device *flash_dev = fal_blk_device_create(FS_PARTITION_NAME);
    if (flash_dev == NULL)
    {
        LOG_E("Can't create a block device on '%s' partition.", FS_PARTITION_NAME);
    }
    else
    {
        LOG_D("Create a block device on the %s partition of flash successful.", FS_PARTITION_NAME);
    }

    /* 挂载 spi flash 中名为 "filesystem" 的分区上的文件系统 */
    if (dfs_mount(flash_dev->parent.name, "/fal", "elm", 0, 0) == 0)
    {
        LOG_I("Filesystem initialized!");
    }
    else
    {
        LOG_E("Failed to initialize filesystem!");
        LOG_D("You should create a filesystem on the block device first!");
    }
#endif
}

static int SPI_Flash_fal_test(int argc, char *argv[])
{
    if (argc != 2)
    {
        LOG_W("Usage: %s <partiton_name>", argv[0]);
        return RT_ERROR;
    }

    int ret;
    int i, j, len;
    uint8_t buf[BUF_SIZE];
    const struct fal_flash_dev *flash_dev = RT_NULL;
    const struct fal_partition *partition = RT_NULL;

    if (!argv[1])
    {
        LOG_E("Input param partition name is null!");
        return -1;
    }

    partition = fal_partition_find(argv[1]);
    if (partition == RT_NULL)
    {
        LOG_E("Find partition (%s) failed!", argv[1]);
        ret = -1;
        return ret;
    }

    flash_dev = fal_flash_device_find(partition->flash_name);
    if (flash_dev == RT_NULL)
    {
        LOG_E("Find flash device (%s) failed!", partition->flash_name);
        ret = -1;
        return ret;
    }

    LOG_I("Flash device : %s   "
          "Flash size : %dK   "
          "Partition : %s   "
          "Partition size: %dK",
          partition->flash_name,
          flash_dev->len / 1024,
          partition->name,
          partition->len / 1024);

    /* 擦除 `partition` 分区上的全部数据 */
    ret = fal_partition_erase_all(partition);
    if (ret < 0)
    {
        LOG_E("Partition (%s) erase failed!", partition->name);
        ret = -1;
        return ret;
    }
    LOG_I("Erase (%s) partition finish!", argv[1]);

    /* 循环读取整个分区的数据，并对内容进行检验 */
    for (i = 0; i < partition->len;)
    {
        rt_memset(buf, 0x00, BUF_SIZE);
        len = (partition->len - i) > BUF_SIZE ? BUF_SIZE : (partition->len - i);

        /* 从 Flash 读取 len 长度的数据到 buf 缓冲区 */
        ret = fal_partition_read(partition, i, buf, len);
        if (ret < 0)
        {
            LOG_E("Partition (%s) read failed!", partition->name);
            ret = -1;
            return ret;
        }
        for (j = 0; j < len; j++)
        {
            /* 校验数据内容是否为 0xFF */
            if (buf[j] != 0xFF)
            {
                LOG_E("The erase operation did not really succeed!");
                ret = -1;
                return ret;
            }
        }
        i += len;
    }

    /* 把 0 写入指定分区 */
    for (i = 0; i < partition->len;)
    {
        /* 设置写入的数据 0x00 */
        rt_memset(buf, 0x00, BUF_SIZE);
        len = (partition->len - i) > BUF_SIZE ? BUF_SIZE : (partition->len - i);

        /* 写入数据 */
        ret = fal_partition_write(partition, i, buf, len);
        if (ret < 0)
        {
            LOG_E("Partition (%s) write failed!", partition->name);
            ret = -1;
            return ret;
        }
        i += len;
    }
    LOG_I("Write (%s) partition finish! Write size %d(%dK).", argv[1], i, i / 1024);

    /* 从指定的分区读取数据并校验数据 */
    for (i = 0; i < partition->len;)
    {
        /* 清空读缓冲区，以 0xFF 填充 */
        rt_memset(buf, 0xFF, BUF_SIZE);
        len = (partition->len - i) > BUF_SIZE ? BUF_SIZE : (partition->len - i);

        /* 读取数据到 buf 缓冲区 */
        ret = fal_partition_read(partition, i, buf, len);
        if (ret < 0)
        {
            LOG_E("Partition (%s) read failed!", partition->name);
            ret = -1;
            return ret;
        }
        for (j = 0; j < len; j++)
        {
            /* 校验读取的数据是否为步骤 3 中写入的数据 0x00 */
            if (buf[j] != 0x00)
            {
                LOG_E("The write operation did not really succeed!");
                ret = -1;
                return ret;
            }
        }
        i += len;
    }

    ret = 0;
    return ret;
}
MSH_CMD_EXPORT_ALIAS(SPI_Flash_fal_test, spi_flash_test, test spi flash read write function);