// Copyright (c) 2015-2026 The Litonpay Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "arith_uint256.h"
#include "chainparams.h"
#include "litonpay.h"
#include "test/test_bitcoin.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(litonpay_tests, TestingSetup)

/**
 * Litonpay block subsidy tests.
 *
 * Parameters:
 *   - Initial reward: 50 LTP
 *   - Halving interval: 2,850,000 blocks
 *   - Total supply: 285,000,000 LTP (capped)
 */

BOOST_AUTO_TEST_CASE(subsidy_first_phase)
{
    const CChainParams& mainParams = Params(CBaseChainParams::MAIN);
    CAmount nSum = 0;

    // Phase 0: blocks 0 - 2,849,999, reward 50 LTP
    for (int nHeight = 0; nHeight < 2850000; nHeight++) {
        const Consensus::Params& params = mainParams.GetConsensus(nHeight);
        CAmount nSubsidy = GetLitonpayBlockSubsidy(nHeight, params, uint256S("0"));
        BOOST_CHECK(MoneyRange(nSubsidy));
        BOOST_CHECK_EQUAL(nSubsidy, 50 * COIN);
        nSum += nSubsidy;
    }
    BOOST_CHECK_EQUAL(nSum, 142500000 * COIN);  // 50 × 2,850,000
}

BOOST_AUTO_TEST_CASE(subsidy_halving)
{
    const CChainParams& mainParams = Params(CBaseChainParams::MAIN);

    // Block 2,849,999: still 50 LTP
    {
        const Consensus::Params& params = mainParams.GetConsensus(2849999);
        BOOST_CHECK_EQUAL(GetLitonpayBlockSubsidy(2849999, params, uint256S("0")), 50 * COIN);
    }

    // Block 2,850,000: 25 LTP (first halving)
    {
        const Consensus::Params& params = mainParams.GetConsensus(2850000);
        BOOST_CHECK_EQUAL(GetLitonpayBlockSubsidy(2850000, params, uint256S("0")), 25 * COIN);
    }

    // Block 5,700,000: 12.5 LTP (second halving)
    {
        const Consensus::Params& params = mainParams.GetConsensus(5700000);
        BOOST_CHECK_EQUAL(GetLitonpayBlockSubsidy(5700000, params, uint256S("0")), 1250000000);  // 12.5 LTP in satoshis
    }

    // Block 8,550,000: 6.25 LTP (third halving)
    {
        const Consensus::Params& params = mainParams.GetConsensus(8550000);
        BOOST_CHECK_EQUAL(GetLitonpayBlockSubsidy(8550000, params, uint256S("0")), 625000000);
    }
}

BOOST_AUTO_TEST_CASE(subsidy_total_supply)
{
    const CChainParams& mainParams = Params(CBaseChainParams::MAIN);
    CAmount nTotal = 0;

    // Sum total supply until reward becomes zero
    for (int nHeight = 0; nHeight < 2850000 * 64; nHeight += 2850000) {
        const Consensus::Params& params = mainParams.GetConsensus(nHeight);
        CAmount nSubsidy = GetLitonpayBlockSubsidy(nHeight, params, uint256S("0"));
        if (nSubsidy == 0) break;

        // Total in this phase = reward × interval
        nTotal += nSubsidy * 2850000;
    }

    // Total supply should be 285,000,000 LTP
    BOOST_CHECK_EQUAL(nTotal, 285000000 * COIN);
}

BOOST_AUTO_TEST_CASE(subsidy_zero_after_64_halvings)
{
    const CChainParams& mainParams = Params(CBaseChainParams::MAIN);

    // After 64 halvings, reward must be zero
    int nHeight = 2850000 * 64;
    const Consensus::Params& params = mainParams.GetConsensus(nHeight);
    BOOST_CHECK_EQUAL(GetLitonpayBlockSubsidy(nHeight, params, uint256S("0")), 0);
}

BOOST_AUTO_TEST_CASE(get_next_work_difficulty_limit)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus(0);

    CBlockIndex pindexLast;
    int64_t nLastRetargetTime = 1794304800; // Block #1 (Litonpay genesis + 1)

    pindexLast.nHeight = 239;
    pindexLast.nTime = 1794304800 + 239 * 60; // Block #239
    pindexLast.nBits = 0x1e0ffff0;
    BOOST_CHECK_EQUAL(CalculateLitonpayNextWorkRequired(&pindexLast, nLastRetargetTime, params), 0x1e00ffff);
}

BOOST_AUTO_TEST_CASE(get_next_work_pre_digishield)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus(0);

    CBlockIndex pindexLast;
    int64_t nLastRetargetTime = 1794304800 + 9359 * 60; // Block 9359

    pindexLast.nHeight = 9599;
    pindexLast.nTime = 1794304800 + 9599 * 60;
    pindexLast.nBits = 0x1c1a1206;
    BOOST_CHECK_EQUAL(CalculateLitonpayNextWorkRequired(&pindexLast, nLastRetargetTime, params), 0x1c15ea59);
}

BOOST_AUTO_TEST_CASE(get_next_work_digishield)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus(2850000);

    CBlockIndex pindexLast;
    int64_t nLastRetargetTime = 1794304800 + 2850000 * 60;

    // First halving at 2,850,000, which applies to block 2,850,001 onwards
    pindexLast.nHeight = 2850000;
    pindexLast.nTime = 1794304800 + 2850000 * 60;
    pindexLast.nBits = 0x1b499dfd;
    BOOST_CHECK_EQUAL(CalculateLitonpayNextWorkRequired(&pindexLast, nLastRetargetTime, params), 0x1b671062);
}

BOOST_AUTO_TEST_CASE(get_next_work_digishield_modulated_upper)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus(2850000);

    CBlockIndex pindexLast;
    int64_t nLastRetargetTime = 1794304800 + 2850107 * 60;

    // Test the upper bound on modulated time using block #2,850,107
    pindexLast.nHeight = 2850107;
    pindexLast.nTime = 1794304800 + 2850107 * 60;
    pindexLast.nBits = 0x1b3439cd;
    BOOST_CHECK_EQUAL(CalculateLitonpayNextWorkRequired(&pindexLast, nLastRetargetTime, params), 0x1b4e56b3);
}

BOOST_AUTO_TEST_CASE(get_next_work_digishield_modulated_lower)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus(2850000);

    CBlockIndex pindexLast;
    int64_t nLastRetargetTime = 1794304800 + 2854423 * 60;

    // Test the lower bound on modulated time using block #2,854,423
    pindexLast.nHeight = 2854423;
    pindexLast.nTime = 1794304800 + 2854423 * 60;
    pindexLast.nBits = 0x1b446f21;
    BOOST_CHECK_EQUAL(CalculateLitonpayNextWorkRequired(&pindexLast, nLastRetargetTime, params), 0x1b335358);
}

BOOST_AUTO_TEST_CASE(get_next_work_digishield_rounding)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus(2850000);

    CBlockIndex pindexLast;
    int64_t nLastRetargetTime = 1794304800 + 2850000 * 60;

    // Test case for correct rounding of modulated time
    pindexLast.nHeight = 2850001;
    pindexLast.nTime = 1794304800 + 2850001 * 60;
    pindexLast.nBits = 0x1b671062;
    BOOST_CHECK_EQUAL(CalculateLitonpayNextWorkRequired(&pindexLast, nLastRetargetTime, params), 0x1b6558a4);
}

BOOST_AUTO_TEST_CASE(hardfork_parameters)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& initialParams = Params().GetConsensus(0);

    BOOST_CHECK_EQUAL(initialParams.nPowTargetTimespan, 14400);
    BOOST_CHECK_EQUAL(initialParams.fAllowLegacyBlocks, true);
    BOOST_CHECK_EQUAL(initialParams.fDigishieldDifficultyCalculation, false);

    const Consensus::Params& initialParamsEnd = Params().GetConsensus(2849999);
    BOOST_CHECK_EQUAL(initialParamsEnd.nPowTargetTimespan, 14400);
    BOOST_CHECK_EQUAL(initialParamsEnd.fAllowLegacyBlocks, true);
    BOOST_CHECK_EQUAL(initialParamsEnd.fDigishieldDifficultyCalculation, false);

    const Consensus::Params& digishieldParams = Params().GetConsensus(2850000);
    BOOST_CHECK_EQUAL(digishieldParams.nPowTargetTimespan, 60);
    BOOST_CHECK_EQUAL(digishieldParams.fAllowLegacyBlocks, true);
    BOOST_CHECK_EQUAL(digishieldParams.fDigishieldDifficultyCalculation, true);

    const Consensus::Params& digishieldParamsEnd = Params().GetConsensus(371336);
    BOOST_CHECK_EQUAL(digishieldParamsEnd.nPowTargetTimespan, 60);
    BOOST_CHECK_EQUAL(digishieldParamsEnd.fAllowLegacyBlocks, true);
    BOOST_CHECK_EQUAL(digishieldParamsEnd.fDigishieldDifficultyCalculation, true);

    const Consensus::Params& auxpowParams = Params().GetConsensus(371337);
    BOOST_CHECK_EQUAL(auxpowParams.nHeightEffective, 371337);
    BOOST_CHECK_EQUAL(auxpowParams.nPowTargetTimespan, 60);
    BOOST_CHECK_EQUAL(auxpowParams.fAllowLegacyBlocks, false);
    BOOST_CHECK_EQUAL(auxpowParams.fDigishieldDifficultyCalculation, true);

    const Consensus::Params& auxpowHighParams = Params().GetConsensus(700000);
    BOOST_CHECK_EQUAL(auxpowHighParams.nPowTargetTimespan, 60);
    BOOST_CHECK_EQUAL(auxpowHighParams.fAllowLegacyBlocks, false);
    BOOST_CHECK_EQUAL(auxpowHighParams.fDigishieldDifficultyCalculation, true);
}

BOOST_AUTO_TEST_SUITE_END()
