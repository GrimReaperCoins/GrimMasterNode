// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (   0, uint256("0x0000007f8de54d2b86a026c459a86e69f02af29ecccd63aa2731d68e5456bef3") )
        (   1, uint256("0x36200a3ebe279c705f6f11e2b65a01340811ad4f39ff917a53b064f4bb0bc7ad") )
        (   2, uint256("0x57164b11002a6098eda3f696b9b4e669754eeb8b594755734a9ae9e7fac3bccd") )
        (   3, uint256("0x7dfe0aec3656d08b0cbde6eb5c48f0e12d864f62e646d0c5527ad7fb726c02a1") )
        (   4, uint256("0xb5ba9056998fe5f938cd5bd00d5dbc56f17a7d21d48168a46823c8e4516f24eb") )
        (   5, uint256("0x8ba2a4c764f159475857124a4d141f523690dea0b0d8f006ba076a1a86df417c") )
        (   6, uint256("0x6baeb87bb12327e7b5f854cae2879af055adf5022df48074decb72333be063b8") )
        (   7, uint256("0x89f7fbd69a827c85eff9925ac0bc27b83ce13c30d5cd2cfedd8fca016ced53ce") )
        (   8, uint256("0x474249cf9e356f60b1ed33eedfdb832b51ee30fd3253881a6e5be2c9187d136d") )
        (   9, uint256("0xdca68887d3f4e7b88ccbd6f78bf749a110661d6c8371fe1fc8aa466fbc7e8e78") )
        (  10, uint256("0x0226f6ba744585edcd2d4a26842dfdae8f1ca5455ecb1859757301c899b911d5") )
		
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
