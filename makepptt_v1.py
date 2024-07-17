#!/usr/bin/env python3

import os
import sys
import struct as st
from typing import Literal, Optional


class Chunk:
    offset: int

    def pack(self) -> bytes:
        raise NotImplemented("interface")

    def getLength(self) -> int:
        raise NotImplemented("interface")


class PPTT(Chunk):
    items: list["Item"]

    def __init__(
        self,
        signature: str = "PPTT",
        revision: int = 3,
        oemId: str = "DIXYES",
        oemTableId: str = "PPTT    ",
        oemRevision: int = 0,
        creatorId: str = "DIXY",
        creatorRevison: int = 0,
    ):
        self.signature = signature
        self.revision = revision
        self.oemId = oemId
        self.oemTableId = oemTableId
        self.oemRevision = oemRevision
        self.creatorId = creatorId
        self.creatorRevison = creatorRevison

        self.offset = 0
        self.items = []

    def getLength(self) -> int:
        return 36 + sum([item.getLength() for item in self.items])

    def pack(self) -> bytes:
        offset = self.offset + 36
        data = b""

        # build header
        data += st.pack(
            "<4sIBB6s8sI4sI",
            self.signature.encode(),
            self.getLength(),
            self.revision,
            0,  # checksum here,
            self.oemId.encode(),
            self.oemTableId.encode(),
            self.oemRevision,
            self.creatorId.encode(),
            self.creatorRevison,
        )

        # build items
        for item in self.items:
            item.offset = offset
            offset += item.getLength()

        for item in self.items:
            data += item.pack()

        checksum = 0
        for char in data:
            checksum += char
            checksum &= 0xFF

        data = data[:9] + st.pack("<B", 0x100 - checksum) + data[10:]

        return data


class Item(Chunk):
    pass


class ProcessorHierarchyV1(Item):
    privateResources: list[Chunk]

    def __init__(
        self,
        flags: int,
        parent: Optional["ProcessorHierarchyV1"] = None,
        acpiProcessorId: Optional[int] = None,
    ):
        self.flags = flags
        self.parent = parent
        self.acpiProcessorId = acpiProcessorId
        self.privateResources = []

    def getLength(self) -> int:
        return 20 + 4 * len(self.privateResources)

    def pack(self) -> bytes:
        data = b""

        data += st.pack(
            "<BBHIIII",
            0,  # type
            self.getLength(),
            0,  # reserved
            self.flags,
            self.parent and self.parent.offset or 0,
            self.acpiProcessorId or 0,
            len(self.privateResources),
        )
        for resource in self.privateResources:
            data += st.pack("<I", resource.offset)

        return data

class ProcessorHierarchyV3(Item):
    privateResources: list[Chunk]

    def __init__(
        self,
        flags: int,
        parent: Optional["ProcessorHierarchyV3"] = None,
        acpiProcessorId: Optional[int] = None,
    ):
        # 0x8: node is leaf
        # 0x2: acpi procesor id is valid
        if flags & 0x8:
            if not (flags & 0x2):
                raise ValueError(
                    "acpi processor id is valid flag 0x2 is required for leaf node in flags"
                )
            if acpiProcessorId is None:
                raise ValueError("acpi processor id is required for leaf node")
            if parent is None:
                raise ValueError("parent is required for leaf node")
        self.flags = flags
        self.parent = parent
        self.acpiProcessorId = acpiProcessorId
        self.privateResources = []

    def getLength(self) -> int:
        return 20 + 4 * len(self.privateResources)

    def pack(self) -> bytes:
        data = b""

        data += st.pack(
            "<BBHIIII",
            0,  # type
            self.getLength(),
            0,  # reserved
            self.flags,
            self.parent and self.parent.offset or 0,
            self.acpiProcessorId or 0,
            len(self.privateResources),
        )
        for resource in self.privateResources:
            data += st.pack("<I", resource.offset)

        return data

class CacheV1(Item):
    def __init__(
        self,
        nextLevelOfCache: Optional["CacheV1"] = None,
        size: Optional[int] = None,
        numberOfSets: Optional[int] = None,
        associativity: Optional[int] = None,
        allocationType: Optional[Literal[0, 1, 2, 3]] = None,
        cacheType: Optional[Literal[0, 1, 2, 3]] = None,
        writePolicy: Optional[Literal[0, 1]] = None,
        lineSize: Optional[int] = None,
    ):
        self.nextLevelOfCache = nextLevelOfCache
        self.size = size
        self.numberOfSets = numberOfSets
        self.associativity = associativity
        self.allocationType = allocationType
        self.cacheType = cacheType
        self.writePolicy = writePolicy
        self.lineSize = lineSize

    def getLength(self) -> int:
        return 24

    def pack(self) -> bytes:
        flags = 0
        attributes = 0
        if self.size is not None:
            flags |= 0x1
        if self.numberOfSets is not None:
            flags |= 0x2
        if self.associativity is not None:
            flags |= 0x4
        if self.allocationType is not None:
            flags |= 0x8
            attributes |= self.allocationType
        if self.cacheType is not None:
            flags |= 0x10
            attributes |= self.cacheType << 2
        if self.writePolicy is not None:
            flags |= 0x20
            attributes |= self.writePolicy << 4
        if self.lineSize is not None:
            flags |= 0x40

        data = st.pack(
            "<BBHIIIIBBH",
            1,  # type
            self.getLength(),
            0,  # reserved
            flags,
            self.nextLevelOfCache and self.nextLevelOfCache.offset or 0,
            self.size or 0,
            self.numberOfSets or 0,
            self.associativity or 0,
            attributes,
            self.lineSize or 0,
        )

        return data

class CacheV3(Item):
    def __init__(
        self,
        nextLevelOfCache: Optional["CacheV3"] = None,
        size: Optional[int] = None,
        numberOfSets: Optional[int] = None,
        associativity: Optional[int] = None,
        allocationType: Optional[Literal[0, 1, 2, 3]] = None,
        cacheType: Optional[Literal[0, 1, 2, 3]] = None,
        writePolicy: Optional[Literal[0, 1]] = None,
        lineSize: Optional[int] = None,
        cacheId: Optional[int] = None,
    ):
        self.nextLevelOfCache = nextLevelOfCache
        self.size = size
        self.numberOfSets = numberOfSets
        self.associativity = associativity
        self.allocationType = allocationType
        self.cacheType = cacheType
        self.writePolicy = writePolicy
        self.lineSize = lineSize
        self.cacheId = cacheId

    def getLength(self) -> int:
        return 28

    def pack(self) -> bytes:
        flags = 0
        attributes = 0
        if self.size is not None:
            flags |= 0x1
        if self.numberOfSets is not None:
            flags |= 0x2
        if self.associativity is not None:
            flags |= 0x4
        if self.allocationType is not None:
            flags |= 0x8
            attributes |= self.allocationType
        if self.cacheType is not None:
            flags |= 0x10
            attributes |= self.cacheType << 2
        if self.writePolicy is not None:
            flags |= 0x20
            attributes |= self.writePolicy << 4
        if self.lineSize is not None:
            flags |= 0x40
        if self.cacheId is not None:
            flags |= 0x80

        data = st.pack(
            "<BBHIIIIBBHI",
            1,  # type
            self.getLength(),
            0,  # reserved
            flags,
            self.nextLevelOfCache and self.nextLevelOfCache.offset or 0,
            self.size or 0,
            self.numberOfSets or 0,
            self.associativity or 0,
            attributes,
            self.lineSize or 0,
            self.cacheId or 0,
        )

        return data


class ID(Item):
    def __init__(
        self,
        vendorId: str,
        level1Id: int,
        level2Id: int,
        majorVersion: int,
        minorVersion: int,
        spinVersion: int,
    ):
        self.vendorId = vendorId
        self.level1Id = level1Id
        self.level2Id = level2Id
        self.majorVersion = majorVersion
        self.minorVersion = minorVersion
        self.spinVersion = spinVersion

        self.offset = 0

    def getLength(self) -> int:
        return 30

    def pack(self) -> bytes:
        data = st.pack(
            "<BBH4sQQHHH",
            2,  # type
            self.getLength(),
            0,  # reserved
            self.vendorId.encode(),
            self.level1Id,
            self.level2Id,
            self.majorVersion,
            self.minorVersion,
            self.spinVersion,
        )
        return data


class Type98h(Item):
    def __init__(
        self,
        data: bytes,
    ):
        self.data = data

    def getLength(self) -> int:
        return 2 + len(self.data)

    def pack(self) -> bytes:
        data = st.pack(
            "<BB",
            0x98,  # type
            self.getLength(),
        )
        data += self.data
        return data


def main():
    pptt = PPTT(
        revision=1,
        oemId='ARMLTD',
        oemTableId='ARM-PHYT',
        oemRevision=0x20180509,
        creatorId='ARM ',
        creatorRevison=0x99,
    )

    acpiProcessorId = 0

    socket = ProcessorHierarchyV1(
        flags=0x1,  # Physical package
    )
    pptt.items.append(socket)

    id = ID(
        vendorId="PHYT",
        level1Id=0,
        level2Id=0,
        majorVersion=0,
        minorVersion=0,
        spinVersion=0,
    )
    pptt.items.append(id)
    socket.privateResources.append(id)

    for i in range(16):
        cluster = ProcessorHierarchyV1(
            flags=0,
            parent=socket,
        )
        pptt.items.append(cluster)

        l2Cache = CacheV1(
            size=0x200000, # 2MB
            numberOfSets=0x2000,
            associativity=0x10,
            allocationType=2,
            cacheType=2,
            writePolicy=0,
            lineSize=0x80,
        )
        pptt.items.append(l2Cache)
        cluster.privateResources.append(l2Cache)

        for j in range(4):
            core = ProcessorHierarchyV1(
                flags=0x2, # acpi processor id is valid
                parent=cluster,
                acpiProcessorId=acpiProcessorId,
            )
            pptt.items.append(core)

            acpiProcessorId += 1

            l1i = CacheV1(
                size=0x8000, # 32KB
                numberOfSets=0x100,
                associativity=0x2,
                allocationType=0,
                cacheType=1,
                writePolicy=0,
                lineSize=0x40,
            )
            pptt.items.append(l1i)
            core.privateResources.append(l1i)

            l1d = CacheV1(
                size=0x8000, # 32KB
                numberOfSets=0x100,
                associativity=0x2,
                allocationType=2,
                cacheType=0,
                writePolicy=0,
                lineSize=0x40,
            )
            pptt.items.append(l1d)
            core.privateResources.append(l1d)


    open("pptt.aml", "wb").write(pptt.pack())


if __name__ == "__main__":
    main()

