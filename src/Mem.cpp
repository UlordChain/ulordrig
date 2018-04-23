/* UlordRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2017-2018 XMR-Stak    <https://github.com/fireice-uk>, <https://github.com/psychocrypt>
 * Copyright 2018      Lee Clagett <https://github.com/vtnerd>
 * Copyright 2016-2018 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 * Copyright 2018      UlordRig    <https://github.com/UlordChain/ulordrig>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <memory.h>


#include "crypto/cryptoHello.h"
#include "Mem.h"
#include "Options.h"
#include "ulordrig.h"


// bool Mem::m_doubleHash             = false;
int Mem::m_algo                    = 0;
int Mem::m_flags                   = 0;
int Mem::m_threads                 = 0;
size_t Mem::m_offset               = 0;
size_t Mem::m_size                 = 0;
alignas(16) uint8_t *Mem::m_memory = nullptr;

uint8_t* Mem::create(int threadId)
{
    return  &m_memory[WORK_MEMORY_SIZE * (threadId /* + 1 */)];
}



void *Mem::calloc(size_t num, size_t size)
{
    void *mem = &m_memory[m_offset];
    m_offset += (num * size);

    memset(mem, 0, num * size);

    return mem;
}

