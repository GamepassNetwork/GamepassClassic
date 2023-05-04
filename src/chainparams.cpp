// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021 The Raven Core developers
// Copyright (c) 2022-2023 The Gamepass Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include "chainparamsseeds.h"

//TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
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

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
	const char* pszTimestamp = "Are Premium Bonds a good place for your cash?";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
        consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
        consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
        consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
        consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
        consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
        return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
        return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66() {
        return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const{
        return consensus.nCSVEnabled;
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
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 43750;  //~ 1 month at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true;
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled 	= true;
        consensus.powLimit 		= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit 	= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1613; // Approx 80% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1682910000; // Mon May 01 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1682996400; // Tue May 02 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (RIP2)
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1682910000; // Mon May 01 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1682996400; // Tue May 02 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1682910000; // Mon May 01 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1682996400; // Tue May 02 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1682910000; // Mon May 01 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1682996400; // Tue May 02 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1682910000; // Mon May 01 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1682996400; // Tue May 02 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1682910000; // Mon May 01 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1682996400; // Tue May 02 2023 03:00:00 GMT+0000
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;

        consensus.BIP34LockedIn = 1000; // Locked_in at height 1000

        // The best chain should have at least this much work
        consensus.nMinimumChainWork = uint256S("00000000000000000000000000000000000000000000000000b45ba330631843"); // Block 7246

        // By default assume that the signatures in ancestors of this block are valid. Block#
        consensus.defaultAssumeValid = uint256S("00000000000c9dcb50d8ebe35ae33849952cbe7e9e396e05e63d197cabb586bb"); // Block 7246

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x47; // G
        pchMessageStart[1] = 0x50; // P
        pchMessageStart[2] = 0x4e; // N
        pchMessageStart[3] = 0x43; // C
        nDefaultPort = 5315;
        nPruneAfterHeight = 100000;

        
        // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
        /////////////////////////////////////////////////////////////////
        /*
         uint32_t nGenesisTime = 1682766060;
		 arith_uint256 test;
         bool fNegative;
         bool fOverflow;
         test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
         std::cout << "Test threshold: " << test.GetHex() << "\n\n";
         int genesisNonce = 0;
         uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
         uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
         for (int i=0;i<40000000;i++) {
             genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 4, 2000 * COIN);
             genesis.hashPrevBlock = TempHashHolding;
        // Depending on when the timestamp is on the genesis block. You will need to use GetX16RHash or GetX16RV2Hash. Replace GetHash() with these below
             consensus.hashGenesisBlock = genesis.GetX16RHash();
             arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
             if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
                 BestBlockHash = consensus.hashGenesisBlock;
                 std::cout << "CHOOSE: consensus.hashGenesisBlock.GetHex(): " << consensus.hashGenesisBlock.GetHex() << " Nonce: " << i << " nGenesisTime: " << nGenesisTime << "\n";
                 std::cout << "CHOOSE: BestBlockHash.GetHex(): " << BestBlockHash.GetHex() << " Nonce: " << i << " nGenesisTime: " << nGenesisTime << "\n";
                 std::cout << "CHOOSE: genesis.GetX16RHash().GetHex(): " << genesis.GetX16RHash().GetHex() << " Nonce: " << i << " nGenesisTime: " << nGenesisTime << "\n";
                 std::cout << "CHOOSE: PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
             }
             //TempHashHolding = consensus.hashGenesisBlock;
             if (BestBlockHashArith < test) {
                 genesisNonce = i - 1;
                 break;
             }
             std::cout << "	 consensus.hashGenesisBlock.GetHex(): " << consensus.hashGenesisBlock.GetHex() << " Nonce: " << i << "\n";
         }
         std::cout << "\n";
         std::cout << "\n";
         std::cout << "\n";
         std::cout << "BestBlockHash.GetHex() to 0x" << BestBlockHash.GetHex() << std::endl;
         std::cout << "genesis.GetX16RHash().GetHex() to 0x" << genesis.GetX16RHash().GetHex() << std::endl;
         std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
         std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
         std::cout << "\n";
         std::cout << "\n";
         int totalHits = 0;
         double totalTime = 0.0;
         for(int x = 0; x < 16; x++) {
             totalHits += algoHashHits[x];
             totalTime += algoHashTotal[x];
             std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
         }
         std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
         genesis.hashPrevBlock = TempHashHolding;
         return;
        // 		*/
        /////////////////////////////////////////////////////////////////

		genesis = CreateGenesisBlock(1682766060, 1551043, 0x1e00ffff, 4, 2000 * COIN);
		consensus.hashGenesisBlock = genesis.GetX16RHash();
		assert(consensus.hashGenesisBlock == uint256S("000000f31a47d3b9d1cc321e051bafc825cc6fb2998f3eb589e1fc61b9f08736"));
		assert(genesis.hashMerkleRoot == uint256S("41d13e86039a70d1002e46e5a29b5d4babdafb1076c62c9a3002ba712c3b9436"));

        vSeeds.emplace_back("seeder.gpseed.me", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,97);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,144);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // Gamepass BIP44 cointype in mainnet is '1511'
        nExtCoinType = 1511;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

                checkpointData = (CCheckpointData) {
            {
                {
                  {0, uint256S("000000f31a47d3b9d1cc321e051bafc825cc6fb2998f3eb589e1fc61b9f08736")},
				  {1331, uint256S("000000f31a47d3b9d1cc321e051bafc825cc6fb2998f3eb589e1fc61b9f08736")},
				  {7246, uint256S("00000000000c9dcb50d8ebe35ae33849952cbe7e9e396e05e63d197cabb586bb")}
                }
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Gamepass chain
            1683216351, // * UNIX timestamp of last known number of transactions 2021-06-18 22:03:06 UTC
            8490,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.02689522964559008       // * estimated number of transactions per second after that timestamp
        };


        // Burn Amounts
        nIssueAssetBurnAmount = 5 * COIN;
        nReissueAssetBurnAmount = 1 * COIN;
        nIssueSubAssetBurnAmount = 1 * COIN;
        nIssueUniqueAssetBurnAmount = .05 * COIN;
        nIssueMsgChannelAssetBurnAmount = 1 * COIN;
        nIssueQualifierAssetBurnAmount = 10 * COIN;
        nIssueSubQualifierAssetBurnAmount = 1 * COIN;
        nIssueRestrictedAssetBurnAmount = 15 * COIN;
        nAddNullQualifierTagBurnAmount = .01 * COIN;


        // PoG/Dev funds amounts
		nProofOfGameplayAmount = 41;
		nDevFundAmount = 8;

        // Burn Addresses
        strIssueAssetBurnAddress = "GcissueAssetBurnXXXXXXXXXXXXTmnMpL";
        strReissueAssetBurnAddress = "GReissueAssetBurnXXXXXXXXXXXXXNamt";
        strIssueSubAssetBurnAddress = "GSubAssetBurnXXXXXXXXXXXXXXXVNFpTz";
        strIssueUniqueAssetBurnAddress = "GUniqueAssetBurnXXXXXXXXXXXXUs9CDd";
        strIssueMsgChannelAssetBurnAddress = "GMsgChanAssetBurnXXXXXXXXXXXWyUvTW";
        strIssueQualifierAssetBurnAddress = "GQuaAssetBurnXXXXXXXXXXXXXXXXhL2F8";
        strIssueSubQualifierAssetBurnAddress = "GSubQuaAssetBurnXXXXXXXXXXXXUmB9oU";
        strIssueRestrictedAssetBurnAddress = "GRestrictedAssetBurnXXXXXXXXajyp19";
        strAddNullQualifierTagBurnAddress = "GZAddNuQuaTagBurnXXXXXXXXXXXZLQW6C";
        //Global Burn Address
        strGlobalBurnAddress = "GLobaLBurnAddressXXXXXXXXXXXTSpJJR";
        //Gamepass funds addresses
        strProofOfGameplayAddress = "GameFundF4j1hr2qQXbcXCb2NXTtzqX1iL";
        strDevFundAddress = "GPNdevu8cmUE6BmUAon6J311WL3cNtGonQ";

        // DGW Activation
        nDGWActivationBlock = 1000;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 20; // Asset activated block height
        nMessagingActivationBlock = 20; // Messaging activated block height
        nRestrictedActivationBlock = 20; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1682900700;
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
    }
};

/**
 * Testnet (v7)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 43750;  //~ 1month at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; //
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled 	= true;
        consensus.powLimit 	= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit 	= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1613; // Approx 80% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1662998400; // Monday, 12 September 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1665590400; // Wednesday, 12 October 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (RIP2)
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1662998400; // Monday, 12 September 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1665590400; // Wednesday, 12 October 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1662998400; // Monday, 12 September 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1665590400; // Wednesday, 12 October 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1662998400; // Monday, 12 September 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1665590400; // Wednesday, 12 October 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1662998400; // Monday, 12 September 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1665590400; // Wednesday, 12 October 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1662998400; // Monday, 12 September 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1665590400; // Wednesday, 12 October 2022 16:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.BIP34LockedIn = 1000; // Locked_in at height 1000

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // Block 0

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000016829ab234be5321453740c31faf66944519901d8e8d9d73bc1b821039"); // Block 0


        pchMessageStart[0] = 0x47; // G
        pchMessageStart[1] = 0x50; // P
        pchMessageStart[2] = 0x4e; // N
        pchMessageStart[3] = 0x54; // T
        nDefaultPort = 15315;
        nPruneAfterHeight = 1000;

        
        // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
        /////////////////////////////////////////////////////////////////
        /*
		 uint32_t nGenesisTime = 1682769600;
         arith_uint256 test;
         bool fNegative;
         bool fOverflow;
         test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
         std::cout << "Test threshold: " << test.GetHex() << "\n\n";
         int genesisNonce = 0;
         uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
         uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
         for (int i=0;i<40000000;i++) {
             genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 4, 2000 * COIN);
             genesis.hashPrevBlock = TempHashHolding;
        // Depending on when the timestamp is on the genesis block. You will need to use GetX16RHash or GetX16RV2Hash. Replace GetHash() with these below
             consensus.hashGenesisBlock = genesis.GetX16RHash();
             arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
             if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
                 BestBlockHash = consensus.hashGenesisBlock;
                 std::cout << "CHOOSE: consensus.hashGenesisBlock.GetHex(): " << consensus.hashGenesisBlock.GetHex() << " Nonce: " << i << " nGenesisTime: " << nGenesisTime << "\n";
                 std::cout << "CHOOSE: BestBlockHash.GetHex(): " << BestBlockHash.GetHex() << " Nonce: " << i << " nGenesisTime: " << nGenesisTime << "\n";
                 std::cout << "CHOOSE: genesis.GetX16RHash().GetHex(): " << genesis.GetX16RHash().GetHex() << " Nonce: " << i << " nGenesisTime: " << nGenesisTime << "\n";
                 std::cout << "CHOOSE: PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
             }
             //TempHashHolding = consensus.hashGenesisBlock;
             if (BestBlockHashArith < test) {
                 genesisNonce = i - 1;
                 break;
             }
             std::cout << "	 consensus.hashGenesisBlock.GetHex(): " << consensus.hashGenesisBlock.GetHex() << " Nonce: " << i << "\n";
         }
         std::cout << "\n";
         std::cout << "\n";
         std::cout << "\n";
         std::cout << "BestBlockHash.GetHex() to 0x" << BestBlockHash.GetHex() << std::endl;
         std::cout << "genesis.GetX16RHash().GetHex() to 0x" << genesis.GetX16RHash().GetHex() << std::endl;
         std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
         std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
         std::cout << "\n";
         std::cout << "\n";
         int totalHits = 0;
         double totalTime = 0.0;
         for(int x = 0; x < 16; x++) {
             totalHits += algoHashHits[x];
             totalTime += algoHashTotal[x];
             std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
         }
         std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
         genesis.hashPrevBlock = TempHashHolding;
         return;
        // 		*/
        /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(1682769600, 14780927, 0x1e00ffff, 4, 2000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        assert(consensus.hashGenesisBlock == uint256S("00000016829ab234be5321453740c31faf66944519901d8e8d9d73bc1b821039"));
        assert(genesis.hashMerkleRoot == uint256S("41d13e86039a70d1002e46e5a29b5d4babdafb1076c62c9a3002ba712c3b9436"));

        vFixedSeeds.clear();
        vSeeds.clear();

//	      vSeeds.emplace_back("testnet.gamepass.net", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Gamepass BIP44 cointype in testnet
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
				{0, uint256S("00000016829ab234be5321453740c31faf66944519901d8e8d9d73bc1b821039")}
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Gamepass chain
            1682769700, // * UNIX timestamp of last known number of transactions
            1,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.1        // * estimated number of transactions per second after that timestamp
        };

        /** GPN START **/
        // Burn Amounts
        nIssueAssetBurnAmount = 5 * COIN;
        nReissueAssetBurnAmount = 1 * COIN;
        nIssueSubAssetBurnAmount = 1 * COIN;
        nIssueUniqueAssetBurnAmount = .05 * COIN;
        nIssueMsgChannelAssetBurnAmount = 1 * COIN;
        nIssueQualifierAssetBurnAmount = 10 * COIN;
        nIssueSubQualifierAssetBurnAmount = 1 * COIN;
        nIssueRestrictedAssetBurnAmount = 15 * COIN;
        nAddNullQualifierTagBurnAmount = .01 * COIN;

        // PoG/Dev funds amounts
        nProofOfGameplayAmount = 41;
        nDevFundAmount = 8;

        // Burn Addresses
        strIssueAssetBurnAddress = "n1issueAssetBurnXXXXXXXXXXXXSL7nZy";
        strReissueAssetBurnAddress = "n1ReissueAssetBurnXXXXXXXXXXUmPSPC";
        strIssueSubAssetBurnAddress = "n1SubAssetBurnXXXXXXXXXXXXXXXiE8aF";
        strIssueUniqueAssetBurnAddress = "n1UniqueAssetBurnXXXXXXXXXXXaLKSRH";
        strIssueMsgChannelAssetBurnAddress = "n1MsgChanAssetBurnXXXXXXXXXXXdgX2Z";
        strIssueQualifierAssetBurnAddress = "n1QuaAssetBurnXXXXXXXXXXXXXXb9nT2d";
        strIssueSubQualifierAssetBurnAddress = "n1SubQuaAssetBurnXXXXXXXXXXXXveEAE";
        strIssueRestrictedAssetBurnAddress = "n1RestrictedAssetBurnXXXXXXXbHq52U";
        strAddNullQualifierTagBurnAddress = "n1AddNuQuaTagBurnXXXXXXXXXXXWPKK8e";
        //Global Burn Address
        strGlobalBurnAddress = "mfccpXak5JdvUVodmX2Y4LLRZPmLTBroGS";

        //Gamepass funds addresses
        strProofOfGameplayAddress = "mw2UcaNfqfj4xSeJURZUFYg7W51oBhk6Zd";
        strDevFundAddress = "mfe6kYJsTrp4ppopRFHiLZVX8D68HUx9n4";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 1; // Asset activated block height
        nMessagingActivationBlock = 1; // Messaging activated block height
        nRestrictedActivationBlock = 1; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1767377804;
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** GPN END **/
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 0; // GMT: Sun Mar 3, 2019 5:00:00 PM
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 999999999999ULL; // UTC: Wed Dec 25 2019 07:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 208;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 288;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 400;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 500;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x43; // C
        pchMessageStart[1] = 0x52; // R
        pchMessageStart[2] = 0x4F; // O
        pchMessageStart[3] = 0x57; // W
        nDefaultPort = 23632;
        nPruneAfterHeight = 1000;


        genesis = CreateGenesisBlock(1524179366, 1, 0x207fffff, 4, 2000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        //assert(consensus.hashGenesisBlock == uint256S("0x0b2c703dc93bb63a36c4e33b85be4855ddbca2ac951a7a0a29b8de0408200a3c "));
        //assert(genesis.hashMerkleRoot == uint256S("0x28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Gamepass BIP44 cointype in regtest
        nExtCoinType = 1;

        /** GPN START **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;

		// PoG/Dev funds amounts
        nProofOfGameplayAmount = 40;
        nDevFundAmount = 5;

        // Burn Addresses
        strIssueAssetBurnAddress = "n1issueAssetBurnXXXXXXXXXXXXSL7nZy";
        strReissueAssetBurnAddress = "n1ReissueAssetBurnXXXXXXXXXXUmPSPC";
        strIssueSubAssetBurnAddress = "n1SubAssetBurnXXXXXXXXXXXXXXXiE8aF";
        strIssueUniqueAssetBurnAddress = "n1UniqueAssetBurnXXXXXXXXXXXaLKSRH";
        strIssueMsgChannelAssetBurnAddress = "n1MsgChanAssetBurnXXXXXXXXXXXdgX2Z";
        strIssueQualifierAssetBurnAddress = "n1QuaAssetBurnXXXXXXXXXXXXXXb9nT2d";
        strIssueSubQualifierAssetBurnAddress = "n1SubQuaAssetBurnXXXXXXXXXXXXveEAE";
        strIssueRestrictedAssetBurnAddress = "n1RestrictedAssetBurnXXXXXXXbHq52U";
        strAddNullQualifierTagBurnAddress = "n1AddNuQuaTagBurnXXXXXXXXXXXWPKK8e";

        // Global Burn Address
        strGlobalBurnAddress = "n1GLobaLBurnAddressXXXXXXXXXYTqeJm";

        //Gamepass funds addresses
        strProofOfGameplayAddress = "n1GLobaLBurnAddressXXXXXXXXXYTqeJm";
        strDevFundAddress = "n1GLobaLBurnAddressXXXXXXXXXYTqeJm";

        // DGW Activation
        nDGWActivationBlock = 200;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 0; // Asset activated block height
        nMessagingActivationBlock = 0; // Messaging activated block height
        nRestrictedActivationBlock = 0; // Restricted activated block height

        // TODO, we need to figure out what to do with this for regtest. This effects the unit tests
        // For now we can use a timestamp very far away
        // If you are looking to test the kawpow hashing function in regtest. You will need to change this number
        nKAAAWWWPOWActivationTime = 3582830167;
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** GPN END **/
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &GetParams() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network, bool fForceBlockNetwork)
{
    SelectBaseParams(network);
    if (fForceBlockNetwork) {
        bNetwork.SetNetwork(network);
    }
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
        globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
        globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
        globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
        globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
        globalChainParams->TurnOffBIP66();
}
