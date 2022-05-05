#pragma once

#include <fstream>
#include <unordered_map>

#include "proclib.hpp"
#include "datastructs/utils.hpp"
#include "datastructs/utils_pid.hpp"
#include "src/include/filedescriptor.hpp"

#define MAPS "/maps"
#define BUFFER_READ_UTILS 15

/**
 * @brief manipulation of process
 *
 */
class Pmap : protected RemoteProcess
{
public:
    Pmap();
    ~Pmap();

    int map_pid(pid_t );
    bool map_mem(const std::string &);
    
    bool map_write(off_t, void *, uint);
    bool map_read(off_t, uint, Data &);
    int map_find(off_t, uint64_t, std::string, uint8_t, std::vector<off_t> &);
    void map_stop(bool = true);
    void map_kill();
    
    off_t get_addrOn() const;
    off_t get_addrOff() const;
    uint64_t get_sizeAddress();
    std::string get_Flags();
    
    const std::unordered_map<std::string, maps>& get_Maps() const;
    std::string get_utilsPid(uint8_t);
    
private:
    maps  m_maps;
    FileDescriptor m_FS;
    std::string m_maps_buf;
    
    void split_mem_address(std::string &);
    void split_maps();

    std::unordered_map<std::string, maps> m_unmap;

    struct Infos
    {
        pid_t pid;
        pid_t pid_max;

    } m_infos;
};