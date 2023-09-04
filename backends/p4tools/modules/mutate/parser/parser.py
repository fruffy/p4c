#!/usr/bin/env python3

import argparse
import os
import sys

# import matplotlib.pyplot as plt
from collections import OrderedDict
from datetime import datetime


# Utility functions
def displayLog(filename):
    with open(filename, 'r') as fr:
        for line in fr.readlines():
            print(line.rstrip())


def writeLog(filename, logLines):
    with open(filename, 'w') as fw:
        for line in logLines:
            fw.write(line + '\n')


def readLog(filename):
    mLines = []
    with open(filename, 'r') as fread:
        mLines = fread.readlines()
    return mLines


def processLog(modellog):
    mLines = readLog(modellog)
    packets = getPacketParams(mLines)
    return packets


def getPacketParams(mLines):
    packets = []
    testnum = 1
    for i in range(0, len(mLines)):
        if "Begin packet processing" in mLines[i].strip():
            packet = OrderedDict()
            packet['testnum'] = testnum
            # This will be extracted from testgen PTF
            packet['timestamp'] = "Unknown"
            packet['startline'] = i
            packet['parser'] = OrderedDict()
            packet['tables'] = OrderedDict()
            parser = OrderedDict()
            states = OrderedDict()
            tables = OrderedDict()
            prevTable = None
            currGress = None
            for j in range(i + 1, len(mLines)):
                if "Parser state" in mLines[j].strip():
                    if "Ingress " in mLines[j].strip():
                        currGress = "ingress"
                    else:
                        currGress = "egress"
                    pState = mLines[j].strip()
                    pInfo = pState.split("Parser state")[1]
                    st = pInfo.split(' ')[1]
                    if st[-1] == ':':
                        st = st[1:-2]
                        field = pInfo.split(' ')[-1][1:-1]
                        states[st]['fields'].append(field)
                    else:
                        state = OrderedDict()
                        st = st[1:-1]
                        state['name'] = st
                        state['fields'] = []
                        state['gress'] = currGress
                        states[st] = state
                elif "gress : Table" in mLines[j].strip():
                    pTable = mLines[j].strip()
                    pInfo = pTable.split("gress : Table")[1]
                    tbl = pInfo.split(' ')[1]
                    state = pInfo.split(' ')[3]
                    table = OrderedDict()
                    table['name'] = tbl
                    table['state'] = state
                    table['statements'] = []
                    table['gress'] = currGress
                    tables[tbl] = table
                    prevTable = tbl
                elif "Execute" in mLines[j].strip() and "Action" in mLines[j].strip():
                    pAction = mLines[j].strip()
                    pInfo = pAction.split(": ")[-1]
                    if prevTable in tables:
                        tables[prevTable]['action'] = pInfo
                elif "Destination:" in mLines[j].strip():
                    step = 1
                    for step in range(1, 3):
                        stLine = mLines[j + step].strip()
                        if " = " not in stLine:
                            continue
                        if "Next Table" in stLine:
                            break
                        stLine = mLines[j + step].strip()
                        if prevTable in tables:
                            tables[prevTable]['statements'].append(stLine.split('>:')[1].strip())
                    j = j + step
                elif "Waiting for packets to process" in mLines[j].strip():  # End of test
                    parser['states'] = states
                    packet['parser'] = parser
                    packet['tables'] = tables
                    packet['endline'] = j
                    testnum += 1
                    packets.append(packet)
                    i = j + 1
                    break
    return packets


def getParserStatesInfo(states):
    logLines = []
    # logLines.append('Parser States: ')
    # logLines.append('-'*3)
    for k, v in states.items():
        stateInfo = ""
        if 'name' in v:
            stateInfo += v['name']
        if 'gress' in v:
            stateInfo += " " + v['gress']
        if stateInfo != "":
            logLines.append(stateInfo)
        # if 'fields' in v and len(v['fields']) > 0:
        #     logLines.append('Fields:        ')
        #     for f in v['fields']:
        #         logLines.append('               ' + f)
        # logLines.append('-'*3)
    # logLines.append('-'*3)
    return logLines


def getTablesInfo(tables):
    logLines = []
    logLines.append('Tables:         ')
    logLines.append('-' * 3)
    for k, v in tables.items():
        if 'name' in v:
            logLines.append('Table:         ' + v['name'])
        if 'gress' in v:
            logLines.append('Gress:         ' + v['gress'])
        if 'action' in v:
            logLines.append('Action:        ' + v['action'])
        if 'state' in v:
            logLines.append('State:         ' + v['state'])
        if 'statements' in v and len(v['statements']) > 0:
            logLines.append('Statements:    ')
            for st in v['statements']:
                logLines.append('               ' + st)
        logLines.append('-' * 3)
    logLines.append('-' * 3)
    return logLines


def getSummary(packets):
    logLines = []
    # logLines.append('='*3)
    for packet in packets:
        # logLines.append("Packet " + str(packet['testnum']) + " starts at line: " +
        #        str(packet['startline']) + " and ends at line: " + str(packet['endline']))
        logLines.append('-' * 3)
        states = packet['parser']['states']
        logLines.extend(getParserStatesInfo(states))
    ######## NOTE: Only looking at parsers for now
    #     tables = packet['tables']
    #     logLines.extend(getTablesInfo(tables))
    # logLines.append('='*3)
    ########
    logLines.append('-' * 3)

    return logLines


def sanitizeArgs(args):
    if args.modellog is None:
        print('No input files specified. Please specify valid model log.')
        sys.exit()


def extractModelLogInfo(modellog):
    print('=' * 3)
    ts = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
    print(ts, ': Start log parsing.')
    packets = processLog(modellog)
    summary = getSummary(packets)
    ts = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
    summaryLog = 'summary_' + modellog.split('model_')[1]
    writeLog(summaryLog, summary)
    # displayLog(summaryLog)
    print(ts, ': Compileted log parsing.')
    print('=' * 3)
    return packets


def getMatch(tPackets, hTable):
    for tp in tPackets:
        tbInfo = tp['tables']
        if tbInfo == hTable:
            return [tp['testnum'], tp['timestamp']]
    return None


def stripStatements(packets):
    for pnum, pval in enumerate(packets):
        for tb in packets[pnum]['tables']:
            packets[pnum]['tables'][tb]['statements'] = None
    return packets


def parsePTF(testgenptf):
    print("I AM GROOT!!!")
    pLines = readLog(testgenptf)
    tInfo = {}
    for i in range(0, len(pLines)):
        if "Date generated" in pLines[i].strip():
            timestampStr = pLines[i].strip().split(': ')[1]
            timestamp = datetime.strptime(timestampStr, '%Y-%m-%d-%H:%M:%S.%f')
            testnum = pLines[i - 1].strip().split('Test')[1].split('(')[0]
            tInfo[int(testnum)] = int((timestamp.timestamp() * 1000) % 10000)
        i += 1
    return tInfo


def extractTestgenTimestamp(tPackets, testgenptf):
    ptfInfo = parsePTF(testgenptf)
    for tp in tPackets:
        tp['timestamp'] = ptfInfo[tp['testnum']]
    return tPackets


def compareLogs(tPackets, hPackets):
    # For every hand written PTF generated packet,
    # compare with the testgen PTF generated packet,
    # if a match occurs, record the test num of,
    # the corresponding packet.
    matchesTestnum = {}
    matchesTimestamp = {}
    for hp in hPackets:
        tbInfo = hp['tables']
        match = getMatch(tPackets, tbInfo)
        if match:
            matchesTestnum[hp['testnum']] = match[0]
            matchesTimestamp[hp['testnum']] = match[1]
        else:
            matchesTestnum[hp['testnum']] = None
            matchesTimestamp[hp['testnum']] = 0
    return matchesTestnum, matchesTimestamp


def displayMatches(matchesTestnum, matchesTimestamp, totPackets, totHWPackets):
    pktsHit = set()
    hwpktsMiss = set()
    for m1, m2 in matchesTestnum.items():
        if m2:
            pktsHit.add(m2)
            print(
                'Packet',
                m1,
                'corresponds to',
                m2,
                'packet generated at timestamp - ',
                matchesTimestamp[m1],
                '!!!',
            )
        else:
            hwpktsMiss.add(m1)
            print('Packet', m1, 'corresponds to NO Testgen packet!!!')
    numTests = totPackets - len(pktsHit)
    print('No. of unique packets matching testgen PTF - ', len(pktsHit))
    print('No. of extra packet tests in testgen PTF, not covered in handwritten PTF - ', numTests)
    print(
        'Percentage of tests not covered in testgen PTF - ',
        ((len(hwpktsMiss) / totHWPackets) * 100),
    )
    print(
        'Percentage of tests not covered in handwritten tests - ', ((numTests / totPackets) * 100)
    )


def plotTestnumMatches(matches, plotFile):
    plt.plot(matches.keys(), matches.values())
    plt.xlabel('Handwritten PTF tests')
    plt.ylabel('Testgen PTF tests')
    plt.savefig(plotFile)


def plotTimestampMatches(matches, plotFile):
    plt.plot(matches.keys(), matches.values())
    plt.xlabel('Handwritten PTF tests')
    plt.ylabel('Timestamp of testgen generated PTF tests')
    plt.savefig(plotFile)


def printTotalPTFGenTime(tPackets):
    last = len(tPackets)
    timeTaken = tPackets[last - 1]['timestamp'] - tPackets[0]['timestamp']
    print("Total time taken: ", timeTaken)


# Parse the model log and generate a summary.
def main():
    # Parse command line arguments.
    parser = argparse.ArgumentParser(description='Parse a given model log.')
    parser.add_argument('--modellog', type=str, default=None, help='Testgen Model log to parse.')
    parser.add_argument(
        '--comparelog', type=str, default=None, help='Handwritten Model log to parse and compare.'
    )
    parser.add_argument(
        '--testgenptf',
        type=str,
        default=None,
        help='Testgen PTF to extract test generation timestamp.',
    )
    args = parser.parse_args()
    sanitizeArgs(args)
    testgenPackets = extractModelLogInfo(args.modellog)
    if args.comparelog:
        handwrittenPackets = extractModelLogInfo(args.comparelog)
    # Hack
    testgenPackets = stripStatements(testgenPackets)
    if args.comparelog:
        handwrittenPackets = stripStatements(handwrittenPackets)
    # Extract timestamp
    if args.testgenptf:
        testgenPackets = extractTestgenTimestamp(testgenPackets, args.testgenptf)
        matchesTestnum, matchesTimestamp = compareLogs(testgenPackets, handwrittenPackets)
        displayMatches(
            matchesTestnum, matchesTimestamp, len(testgenPackets), len(handwrittenPackets)
        )
        # plotTestnumMatches(matchesTestnum, 'plot_' + args.modellog.split('model_')[1].replace('.log', '_testnum.png'))
        # plotTimestampMatches(matchesTimestamp, 'plot_' + args.modellog.split('model_')[1].replace('.log', '_timestamp.png'))
        # printTotalPTFGenTime(testgenPackets)


if __name__ == "__main__":
    main()
