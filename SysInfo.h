#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <utility>
#include <sys/sysinfo.h>
#include <iostream>

class SysInfo
{
public:

    SysInfo();
	
	//! Функции получения статической информации о ресурсах системы
	virtual uint64_t getRAMTotal() = 0; //!< Пусть все функции занимаемой памяти возвращают размер в байтах пока что
	virtual uint32_t getCPUCoresNum() = 0;
	//! Если ещё придумаете про сеть и диски - добавляйте
    
	//! Функции получения статической информации о самой системе
	std::string getOSFamily(); //!< Windows/Linux/Unix/MacOS/...
    virtual std::string getOSVersion() = 0;
	virtual std::string getOSArch() = 0; //!< x86/x86_64/ARM/...
	virtual std::string getCPUModel() = 0;
    virtual std::string getCPUArch() = 0; //!< x86/x86_64/ARM/...
	
	virtual std::string getVAModel() = 0; //!< VideoAdapter
	virtual uint64_t getVRAMTotal() = 0;



    struct CPUInfo { //Статическая инфа, она будет собираться при создании класса под линукс или виндовс

        struct Core {
            uint64_t virtualId;
            uint64_t physicalId;
            double speed;
            uint64_t cacheSize;
        };

        std::vector<Core> cores;
        std::string model;
        std::string arch;
    };


	struct DriveInfo
	{
		// какие придумаете/сможете получить параметры дисков (HDD/SSD/nvme/flash)
	};
	
	virtual std::vector<DriveInfo> getDrivesInfo() = 0;
	
	struct NetworkAdapterInfo
	{
		// какие придумаете/сможете получить параметры сетевых адаптеров
	};
	
	virtual std::vector<NetworkAdapterInfo> getNetworkAdaptersInfo() = 0;
	
	//! Функции получения динамической информации о ресурсах системы
	virtual double getSystemUptime() = 0;	//!< время в формате double (целая часть - секунды, вещественная - остальное)
	
	virtual uint64_t getRAMFree() = 0;
    virtual uint64_t getRAMUsed() = 0;
	 
    virtual uint32_t getTotalCPUUsage() = 0; //!< Общая от 0 до 100
	virtual std::vector <uint32_t> getCPUCoresUsage() = 0; //!< по ядрам от 0 до 100
    virtual int getCurrentTasksNum() = 0; //!< текущее количество запущенных процессов

	virtual uint64_t getDriveCurrentWrite(std::string driveName) = 0;
	virtual uint64_t getDriveCurrentRead(std::string driveName) = 0;

    virtual uint64_t getNetworkAdapterCurrentUpload(std::string adapterName) = 0;
	virtual uint64_t getNetworkAdapterCurrentDownload(std::string adapterName) = 0;
    
private:
	//! Тут все дополнительные структуры данных, методы и поля класса,
	//! необходимые для получения информации
};