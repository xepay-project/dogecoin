// Copyright (c) 2022 The Litonpay Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Utility functions for RPC commands
 */
#ifndef LITONPAY_WALLET_UTIL_H
#define LITONPAY_WALLET_UTIL_H

#include "fs.h"
#include "util.h"

fs::path GetBackupDirFromInput(std::string strUserFilename);

#endif // LITONPAY_WALLET_UTIL_H
