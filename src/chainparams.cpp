// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2022-2024 The Litonpay Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1386325540, nBits=0x1e0ffff0, nNonce=99943, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "ID Today 01/Nov/2026 Aldianokto, Chaos in the Treasury, Order in the Code";
    const CScript genesisOutputScript = CScript() << ParseHex("040c298a2bd171615ceaeae44729342eb7e0bc4be2ea71c472664d130e83e48ea88bc0b5dc15c188647dd22773f6d69586254c60ea19f7133bdb138aad513cd2b2") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2850000;
        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;
        consensus.BIP34Height = 103438;
        consensus.BIP34Hash = uint256S("0x0");
        consensus.BIP65Height = 346475; // 34cd2cbba4ba366f47e5aa0db5f02c19eba2adf679ceb6653ac003bdc9a0ef1f - first v4 block after the last v3 block
        consensus.BIP66Height = 103438; // 80d1364201e5df97e696c03bdd24dc885e8617b9de51e453c10a4f629b1e797a - this is the last block that could be v2, 1900 blocks past the last v2 block
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3 * 24 * 60 * 60; // pre-digishield: 3 days
        consensus.nPowTargetSpacing = 2.5 * 60; // 2.5 minute
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 100;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Disabled

        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000e993d2aa86cf246a49b"); // 5,050,000
        consensus.defaultAssumeValid = uint256S("0x00000c9d2de90d2aaa4e930e127793f60127745c2fcc19eacb810ce1b97331da"); // 5,050,000

        consensus.nAuxpowChainId = 0x4425;
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;
        consensus.nHeightEffective = 0;

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 145000;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 2.5 * 60; // post-digishield: 2.5 minute
        digishieldConsensus.nCoinbaseMaturity = 150;

        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = 371337;
        auxpowConsensus.fAllowLegacyBlocks = true;

        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xad;
        pchMessageStart[1] = 0xda;
        pchMessageStart[2] = 0xbd;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 7855;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1793512758, 479559, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x00000c9d2de90d2aaa4e930e127793f60127745c2fcc19eacb810ce1b97331da"));
        assert(genesis.hashMerkleRoot == uint256S("0xd20d7fa0def6add36e043cc03103c069b3ed519feebc352c4080b981d4b14b6c"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        // vSeeds.push_back(CDNSSeedData("multidoge.org", "seed.multidoge.org", true));
        // vSeeds.push_back(CDNSSeedData("multidoge.org", "seed2.multidoge.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,22);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (      0, uint256S("0x00000c9d2de90d2aaa4e930e127793f60127745c2fcc19eacb810ce1b97331da"))
        };

        chainTxData = ChainTxData{
            // Data as of block e7d4577405223918491477db725a393bcfc349d8ee63b0a4fde23cbfbfd81dea (height 5050000).
            // Tx estimate based on average between 2023-01-16 (92752025 at 4556625) and 2024-01-16 (226128837 at 5050000)
            1705383360, // * UNIX timestamp of last checkpoint block
            226128837,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            4.23        // * estimated number of transactions per second after checkpoint
                        // (226128837 - 92752025) / 31536000 = 4.2293509
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nHeightEffective = 0;
        consensus.nPowTargetTimespan = 3 * 24 * 60 * 60; // pre-digishield: 3 days
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 100;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.nSubsidyHalvingInterval = 2850000;
        consensus.nMajorityEnforceBlockUpgrade = 501;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 708658;
        consensus.BIP34Hash = uint256S("0x0");
        consensus.BIP65Height = 185470; // 955bd496d23790aba1ecfacb722b089a6ae7ddabaedf7d8fb0878f48308a71f9
        consensus.BIP66Height = 708658; // 21b8b97dcdb94caa67c7f8f6dbf22e61e0cfe0e46e1fff3528b22864659e9b38 - this is the last block that could be v2, 1900 blocks past the last v2 block
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3 * 24 * 60 * 60; // pre-digishield: 3 days
        consensus.nPowTargetSpacing = 2.5 * 60; // 2.5 minute
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2880; // 2 days (note this is significantly lower than Bitcoin standard)
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Disabled

        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000540f2f62ff9a26"); // 5,900,000
        consensus.defaultAssumeValid = uint256S("0x000007d3ef69fd04d17b85eff4a0ff47ec86997f7c29144a338a8b21d0e0f216"); // 5,900,000

        consensus.nAuxpowChainId = 0x4425;
        consensus.fStrictChainId = false;
        consensus.nHeightEffective = 0;
        consensus.fAllowLegacyBlocks = true;

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 145000;
        digishieldConsensus.nPowTargetTimespan = 2.5 * 60; // post-digishield: 2.5 minute
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fPowAllowMinDifficultyBlocks = false;
        digishieldConsensus.nCoinbaseMaturity = 150;

        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = 157500;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = true;

        auxpowConsensus = minDifficultyConsensus;
        auxpowConsensus.nHeightEffective = 158100;
        auxpowConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        auxpowConsensus.fAllowLegacyBlocks = true;

        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &minDifficultyConsensus;
        minDifficultyConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0xac;
        pchMessageStart[1] = 0xce;
        pchMessageStart[2] = 0xed;
        pchMessageStart[3] = 0xcd;
        nDefaultPort = 8355;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1793513539, 1514122, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x000007d3ef69fd04d17b85eff4a0ff47ec86997f7c29144a338a8b21d0e0f216"));
        assert(genesis.hashMerkleRoot == uint256S("0xd20d7fa0def6add36e043cc03103c069b3ed519feebc352c4080b981d4b14b6c"));

        vSeeds.clear();
        // vSeeds.push_back(CDNSSeedData("jrn.me.uk", "testseed.jrn.me.uk"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,65);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,241);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x000007d3ef69fd04d17b85eff4a0ff47ec86997f7c29144a338a8b21d0e0f216"))
        };

        chainTxData = ChainTxData{
            // Data as of block 199bea6a442310589cbb50a193a30b097c228bd5a0f21af21e4e53dd57c382d3 (height 5,900,000)
            1703511130, // * UNIX timestamp of last checkpoint block
            7391928,    // * total number of transactions between genesis and last checkpoint
            0.04        // * estimated number of transactions per second after that timestamp
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1;
        consensus.nPowTargetTimespan = 3 * 24 * 60 * 60; // pre-digishield: 3 days
        consensus.nPowTargetSpacing = 1; // regtest: 1 second blocks
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // Faster than normal for regtest (2,520 instead of 10,080)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        consensus.nMinimumChainWork = uint256S("0x00");
        consensus.defaultAssumeValid = uint256S("0x00");

        consensus.nAuxpowChainId = 0x4425;
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;

        consensus.fSimplifiedRewards = true;
        consensus.nCoinbaseMaturity = 60;

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 10;
        digishieldConsensus.nPowTargetTimespan = 1;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;

        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.nHeightEffective = 20;

        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        pConsensusRoot = &digishieldConsensus;

        pchMessageStart[0] = 0xfe;
        pchMessageStart[1] = 0xda;
        pchMessageStart[2] = 0xba;
        pchMessageStart[3] = 0xfb;
        nDefaultPort = 15444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1793514379, 3, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x54dac5ea0320648caed796e913888649cf01878580b8760b0f037270ec02f1b0"));
        assert(genesis.hashMerkleRoot == uint256S("0xd20d7fa0def6add36e043cc03103c069b3ed519feebc352c4080b981d4b14b6c"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x54dac5ea0320648caed796e913888649cf01878580b8760b0f037270ec02f1b0"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,60);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

const Consensus::Params *Consensus::Params::GetConsensus(uint32_t nTargetHeight) const {
    if (nTargetHeight < this -> nHeightEffective && this -> pLeft != NULL) {
        return this -> pLeft -> GetConsensus(nTargetHeight);
    } else if (nTargetHeight > this -> nHeightEffective && this -> pRight != NULL) {
        const Consensus::Params *pCandidate = this -> pRight -> GetConsensus(nTargetHeight);
        if (pCandidate->nHeightEffective <= nTargetHeight) {
            return pCandidate;
        }
    }

    // No better match below the target height
    return this;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
