//
//  HOGSource.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 26/05/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

/*
#include "HOGSource.h"

namespace cv
{
  size_t HOGDescriptor::getDescriptorSize() const
  {
    CV_Assert(blockSize.width % cellSize.width == 0 &&
              blockSize.height % cellSize.height == 0);
    CV_Assert((winSize.width - blockSize.width) % blockStride.width == 0 &&
              (winSize.height - blockSize.height) % blockStride.height == 0 );
    return (size_t)nbins*
    (blockSize.width/cellSize.width)*
    (blockSize.height/cellSize.height)*
    ((winSize.width - blockSize.width)/blockStride.width + 1)*
    ((winSize.height - blockSize.height)/blockStride.height + 1);
	}
  
	double HOGDescriptor::getWinSigma() const
	{
    return winSigma >= 0 ? winSigma : (blockSize.width + blockSize.height)/8.;
	}
  
	bool HOGDescriptor::checkDetectorSize() const
	{
    size_t detectorSize = svmDetector.size(), descriptorSize = getDescriptorSize();
    return detectorSize == 0 ||
    detectorSize == descriptorSize ||
    detectorSize == descriptorSize + 1;
	}
  
	void HOGDescriptor::setSVMDetector(const vector<float>& _svmDetector)
  85	{
    86	    svmDetector = _svmDetector;
    87	    CV_Assert( checkDetectorSize() );
    88	}
  89
  90	bool HOGDescriptor::load(const String& filename, const String& objname)
  91	{
    92	    FileStorage fs(filename, FileStorage::READ);
    93	    FileNode obj = !objname.empty() ? fs[objname] : fs.getFirstTopLevelNode();
    94	    if( !obj.isMap() )
      95	        return false;
    96	    FileNodeIterator it = obj["winSize"].begin();
    97	    it >> winSize.width >> winSize.height;
    98	    it = obj["blockSize"].begin();
    99	    it >> blockSize.width >> blockSize.height;
    100	    it = obj["blockStride"].begin();
    101	    it >> blockStride.width >> blockStride.height;
    102	    it = obj["cellSize"].begin();
    103	    it >> cellSize.width >> cellSize.height;
    104	    obj["nbins"] >> nbins;
    105	    obj["derivAperture"] >> derivAperture;
    106	    obj["winSigma"] >> winSigma;
    107	    obj["histogramNormType"] >> histogramNormType;
    108	    obj["L2HysThreshold"] >> L2HysThreshold;
    109	    obj["gammaCorrection"] >> gammaCorrection;
    110
    111	    FileNode vecNode = obj["SVMDetector"];
    112	    if( vecNode.isSeq() )
      113	    {
        114	        vecNode >> svmDetector;
        115	        CV_Assert(checkDetectorSize());
        116	    }
    117	    return true;
    118	}
  119
  120	void HOGDescriptor::save(const String& filename, const String& objName) const
  121	{
    122	    FileStorage fs(filename, FileStorage::WRITE);
    123	    fs << (!objName.empty() ? objName : FileStorage::getDefaultObjectName(filename)) << "{";
    124
    125	    fs  << "winSize" << winSize
    126	        << "blockSize" << blockSize
    127	        << "blockStride" << blockStride
    128	        << "cellSize" << cellSize
    129	        << "nbins" << nbins
    130	        << "derivAperture" << derivAperture
    131	        << "winSigma" << getWinSigma()
    132	        << "histogramNormType" << histogramNormType
    133	        << "L2HysThreshold" << L2HysThreshold
    134	        << "gammaCorrection" << gammaCorrection;
    135	    if( !svmDetector.empty() )
    136	        fs << "SVMDetector" << "[:" << svmDetector << "]";
    137	    fs << "}";
    138	}
    139
    140	void HOGDescriptor::computeGradient(const Mat& img, Mat& grad, Mat& qangle,
                                            141	                                    Size paddingTL, Size paddingBR) const
    142	{
      143	    CV_Assert( img.type() == CV_8U || img.type() == CV_8UC3 );
      144
      145	    Size gradsize(img.cols + paddingTL.width + paddingBR.width,
                            146	                  img.rows + paddingTL.height + paddingBR.height);
      147	    grad.create(gradsize, CV_32FC2);  // <magnitude*(1-alpha), magnitude*alpha>
      148	    qangle.create(gradsize, CV_8UC2); // [0..nbins-1] - quantized gradient orientation
      149	    Size wholeSize;
      150	    Point roiofs;
      151	    img.locateROI(wholeSize, roiofs);
      152
      153	    int i, x, y;
      154	    int cn = img.channels();
      155
      156	    Mat_<float> _lut(1, 256);
      157	    const float* lut = &_lut(0,0);
      158
      159	    if( gammaCorrection )
        160	        for( i = 0; i < 256; i++ )
          161	            _lut(0,i) = std::sqrt((float)i);
      162	    else
        163	        for( i = 0; i < 256; i++ )
          164	            _lut(0,i) = (float)i;
      165
      166	    AutoBuffer<int> mapbuf(gradsize.width + gradsize.height + 4);
      167	    int* xmap = (int*)mapbuf + 1;
      168	    int* ymap = xmap + gradsize.width + 2;
      169
      170	    const int borderType = (int)BORDER_REFLECT_101;
      171
      172	    for( x = -1; x < gradsize.width + 1; x++ )
        173	        xmap[x] = borderInterpolate(x - paddingTL.width + roiofs.x,
                                                174	                        wholeSize.width, borderType);
      175	    for( y = -1; y < gradsize.height + 1; y++ )
        176	        ymap[y] = borderInterpolate(y - paddingTL.height + roiofs.y,
                                                177	                        wholeSize.height, borderType);
      178
      179	    // x- & y- derivatives for the whole row
      180	    int width = gradsize.width;
      181	    AutoBuffer<float> _dbuf(width*4);
      182	    float* dbuf = _dbuf;
      183	    Mat Dx(1, width, CV_32F, dbuf);
      184	    Mat Dy(1, width, CV_32F, dbuf + width);
      185	    Mat Mag(1, width, CV_32F, dbuf + width*2);
      186	    Mat Angle(1, width, CV_32F, dbuf + width*3);
      187
      188	    int _nbins = nbins;
      189	    float angleScale = (float)(_nbins/CV_PI);
      190
      191	    for( y = 0; y < gradsize.height; y++ )
        192	    {
          193	        const uchar* imgPtr = img.data + img.step*ymap[y];
          194	        const uchar* prevPtr = img.data + img.step*ymap[y-1];
          195	        const uchar* nextPtr = img.data + img.step*ymap[y+1];
          196	        float* gradPtr = (float*)grad.ptr(y);
          197	        uchar* qanglePtr = (uchar*)qangle.ptr(y);
          198
          199	        if( cn == 1 )
            200	        {
              201	            for( x = 0; x < width; x++ )
                202	            {
                  203	                int x1 = xmap[x];
                  204	                dbuf[x] = (float)(lut[imgPtr[xmap[x+1]]] - lut[imgPtr[xmap[x-1]]]);
                  205	                dbuf[width + x] = (float)(lut[nextPtr[x1]] - lut[prevPtr[x1]]);
                  206	            }
              207	        }
          208	        else
            209	        {
              210	            for( x = 0; x < width; x++ )
                211	            {
                  212	                int x1 = xmap[x]*3;
                  213	                const uchar* p2 = imgPtr + xmap[x+1]*3;
                  214	                const uchar* p0 = imgPtr + xmap[x-1]*3;
                  215	                float dx0, dy0, dx, dy, mag0, mag;
                  216
                  217	                dx0 = lut[p2[2]] - lut[p0[2]];
                  218	                dy0 = lut[nextPtr[x1+2]] - lut[prevPtr[x1+2]];
                  219	                mag0 = dx0*dx0 + dy0*dy0;
                  220
                  221	                dx = lut[p2[1]] - lut[p0[1]];
                  222	                dy = lut[nextPtr[x1+1]] - lut[prevPtr[x1+1]];
                  223	                mag = dx*dx + dy*dy;
                  224
                  225	                if( mag0 < mag )
                    226	                {
                      227	                    dx0 = dx;
                      228	                    dy0 = dy;
                      229	                    mag0 = mag;
                      230	                }
                  231
                  232	                dx = lut[p2[0]] - lut[p0[0]];
                  233	                dy = lut[nextPtr[x1]] - lut[prevPtr[x1]];
                  234	                mag = dx*dx + dy*dy;
                  235
                  236	                if( mag0 < mag )
                    237	                {
                      238	                    dx0 = dx;
                      239	                    dy0 = dy;
                      240	                    mag0 = mag;
                      241	                }
                  242
                  243	                dbuf[x] = dx0;
                  244	                dbuf[x+width] = dy0;
                  245	            }
              246	        }
          247
          248	        cartToPolar( Dx, Dy, Mag, Angle, false );
          249
          250	        for( x = 0; x < width; x++ )
            251	        {
              252	            float mag = dbuf[x+width*2], angle = dbuf[x+width*3]*angleScale - 0.5f;
              253	            int hidx = cvFloor(angle);
              254	            angle -= hidx;
              255	            if( hidx < 0 )
                256	                hidx += _nbins;
              257	            else if( hidx >= _nbins )
                258	                hidx -= _nbins;
              259	            assert( (unsigned)hidx < (unsigned)_nbins );
              260
              261	            qanglePtr[x*2] = (uchar)hidx;
              262	            hidx++;
              263	            hidx &= hidx < _nbins ? -1 : 0;
              264	            qanglePtr[x*2+1] = (uchar)hidx;
              265	            gradPtr[x*2] = mag*(1.f - angle);
              266	            gradPtr[x*2+1] = mag*angle;
              267	        }
          268	    }
      269	}
    270
    271
    272	struct HOGCache
    273	{
      274	    struct BlockData
      275	    {
        276	        BlockData() : histOfs(0), imgOffset() {}
        277	        int histOfs;
        278	        Point imgOffset;
        279	    };
      280
      281	    struct PixData
      282	    {
        283	        size_t gradOfs, qangleOfs;
        284	        int histOfs[4];
        285	        float histWeights[4];
        286	        float gradWeight;
        287	    };
      288
      289	    HOGCache();
      290	    HOGCache(const HOGDescriptor* descriptor,
                       291	        const Mat& img, Size paddingTL, Size paddingBR,
                       292	        bool useCache, Size cacheStride);
      293	    virtual ~HOGCache() {};
      294	    virtual void init(const HOGDescriptor* descriptor,
                                295	        const Mat& img, Size paddingTL, Size paddingBR,
                                296	        bool useCache, Size cacheStride);
      297
      298	    Size windowsInImage(Size imageSize, Size winStride) const;
      299	    Rect getWindow(Size imageSize, Size winStride, int idx) const;
      300
      301	    const float* getBlock(Point pt, float* buf);
      302	    virtual void normalizeBlockHistogram(float* histogram) const;
      303
      304	    vector<PixData> pixData;
      305	    vector<BlockData> blockData;
      306
      307	    bool useCache;
      308	    vector<int> ymaxCached;
      309	    Size winSize, cacheStride;
      310	    Size nblocks, ncells;
      311	    int blockHistogramSize;
      312	    int count1, count2, count4;
      313	    Point imgoffset;
      314	    Mat_<float> blockCache;
      315	    Mat_<uchar> blockCacheFlags;
      316
      317	    Mat grad, qangle;
      318	    const HOGDescriptor* descriptor;
      319	};
    320
    321
    322	HOGCache::HOGCache()
    323	{
      324	    useCache = false;
      325	    blockHistogramSize = count1 = count2 = count4 = 0;
      326	    descriptor = 0;
      327	}
    328
    329	HOGCache::HOGCache(const HOGDescriptor* _descriptor,
                           330	        const Mat& _img, Size _paddingTL, Size _paddingBR,
                           331	        bool _useCache, Size _cacheStride)
    332	{
      333	    init(_descriptor, _img, _paddingTL, _paddingBR, _useCache, _cacheStride);
      334	}
    335
    336	void HOGCache::init(const HOGDescriptor* _descriptor,
                            337	        const Mat& _img, Size _paddingTL, Size _paddingBR,
                            338	        bool _useCache, Size _cacheStride)
    339	{
      340	    descriptor = _descriptor;
      341	    cacheStride = _cacheStride;
      342	    useCache = _useCache;
      343
      344	    descriptor->computeGradient(_img, grad, qangle, _paddingTL, _paddingBR);
      345	    imgoffset = _paddingTL;
      346
      347	    winSize = descriptor->winSize;
      348	    Size blockSize = descriptor->blockSize;
      349	    Size blockStride = descriptor->blockStride;
      350	    Size cellSize = descriptor->cellSize;
      351	    Size winSize = descriptor->winSize;
      352	    int i, j, nbins = descriptor->nbins;
      353	    int rawBlockSize = blockSize.width*blockSize.height;
      354
      355	    nblocks = Size((winSize.width - blockSize.width)/blockStride.width + 1,
                             356	                   (winSize.height - blockSize.height)/blockStride.height + 1);
      357	    ncells = Size(blockSize.width/cellSize.width, blockSize.height/cellSize.height);
      358	    blockHistogramSize = ncells.width*ncells.height*nbins;
      359
      360	    if( useCache )
        361	    {
          362	        Size cacheSize((grad.cols - blockSize.width)/cacheStride.width+1,
                                     363	                       (winSize.height/cacheStride.height)+1);
          364	        blockCache.create(cacheSize.height, cacheSize.width*blockHistogramSize);
          365	        blockCacheFlags.create(cacheSize);
          366	        size_t i, cacheRows = blockCache.rows;
          367	        ymaxCached.resize(cacheRows);
          368	        for( i = 0; i < cacheRows; i++ )
            369	            ymaxCached[i] = -1;
          370	    }
      371
      372	    Mat_<float> weights(blockSize);
      373	    float sigma = (float)descriptor->getWinSigma();
      374	    float scale = 1.f/(sigma*sigma*2);
      375
      376	    for(i = 0; i < blockSize.height; i++)
        377	        for(j = 0; j < blockSize.width; j++)
          378	        {
            379	            float di = i - blockSize.height*0.5f;
            380	            float dj = j - blockSize.width*0.5f;
            381	            weights(i,j) = std::exp(-(di*di + dj*dj)*scale);
            382	        }
      383
      384	    blockData.resize(nblocks.width*nblocks.height);
      385	    pixData.resize(rawBlockSize*3);
      386
      387	    // Initialize 2 lookup tables, pixData & blockData.
      388	    // Here is why:
      389	    //
      390	    // The detection algorithm runs in 4 nested loops (at each pyramid layer):
      391	    //  loop over the windows within the input image
      392	    //    loop over the blocks within each window
      393	    //      loop over the cells within each block
      394	    //        loop over the pixels in each cell
      395	    //
      396	    // As each of the loops runs over a 2-dimensional array,
      397	    // we could get 8(!) nested loops in total, which is very-very slow.
      398	    //
      399	    // To speed the things up, we do the following:
      400	    //   1. loop over windows is unrolled in the HOGDescriptor::{compute|detect} methods;
      401	    //         inside we compute the current search window using getWindow() method.
      402	    //         Yes, it involves some overhead (function call + couple of divisions),
      403	    //         but it's tiny in fact.
      404	    //   2. loop over the blocks is also unrolled. Inside we use pre-computed blockData[j]
      405	    //         to set up gradient and histogram pointers.
      406	    //   3. loops over cells and pixels in each cell are merged
      407	    //       (since there is no overlap between cells, each pixel in the block is processed once)
      408	    //      and also unrolled. Inside we use PixData[k] to access the gradient values and
      409	    //      update the histogram
      410	    //
      411	    count1 = count2 = count4 = 0;
      412	    for( j = 0; j < blockSize.width; j++ )
        413	        for( i = 0; i < blockSize.height; i++ )
          414	        {
            415	            PixData* data = 0;
            416	            float cellX = (j+0.5f)/cellSize.width - 0.5f;
            417	            float cellY = (i+0.5f)/cellSize.height - 0.5f;
            418	            int icellX0 = cvFloor(cellX);
            419	            int icellY0 = cvFloor(cellY);
            420	            int icellX1 = icellX0 + 1, icellY1 = icellY0 + 1;
            421	            cellX -= icellX0;
            422	            cellY -= icellY0;
            423
            424	            if( (unsigned)icellX0 < (unsigned)ncells.width &&
                               425	                (unsigned)icellX1 < (unsigned)ncells.width )
              426	            {
                427	                if( (unsigned)icellY0 < (unsigned)ncells.height &&
                                       428	                    (unsigned)icellY1 < (unsigned)ncells.height )
                  429	                {
                    430	                    data = &pixData[rawBlockSize*2 + (count4++)];
                    431	                    data->histOfs[0] = (icellX0*ncells.height + icellY0)*nbins;
                    432	                    data->histWeights[0] = (1.f - cellX)*(1.f - cellY);
                    433	                    data->histOfs[1] = (icellX1*ncells.height + icellY0)*nbins;
                    434	                    data->histWeights[1] = cellX*(1.f - cellY);
                    435	                    data->histOfs[2] = (icellX0*ncells.height + icellY1)*nbins;
                    436	                    data->histWeights[2] = (1.f - cellX)*cellY;
                    437	                    data->histOfs[3] = (icellX1*ncells.height + icellY1)*nbins;
                    438	                    data->histWeights[3] = cellX*cellY;
                    439	                }
                440	                else
                  441	                {
                    442	                    data = &pixData[rawBlockSize + (count2++)];
                    443	                    if( (unsigned)icellY0 < (unsigned)ncells.height )
                      444	                    {
                        445	                        icellY1 = icellY0;
                        446	                        cellY = 1.f - cellY;
                        447	                    }
                    448	                    data->histOfs[0] = (icellX0*ncells.height + icellY1)*nbins;
                    449	                    data->histWeights[0] = (1.f - cellX)*cellY;
                    450	                    data->histOfs[1] = (icellX1*ncells.height + icellY1)*nbins;
                    451	                    data->histWeights[1] = cellX*cellY;
                    452	                    data->histOfs[2] = data->histOfs[3] = 0;
                    453	                    data->histWeights[2] = data->histWeights[3] = 0;
                    454	                }
                455	            }
            456	            else
              457	            {
                458	                if( (unsigned)icellX0 < (unsigned)ncells.width )
                  459	                {
                    460	                    icellX1 = icellX0;
                    461	                    cellX = 1.f - cellX;
                    462	                }
                463
                464	                if( (unsigned)icellY0 < (unsigned)ncells.height &&
                                       465	                    (unsigned)icellY1 < (unsigned)ncells.height )
                  466	                {
                    467	                    data = &pixData[rawBlockSize + (count2++)];
                    468	                    data->histOfs[0] = (icellX1*ncells.height + icellY0)*nbins;
                    469	                    data->histWeights[0] = cellX*(1.f - cellY);
                    470	                    data->histOfs[1] = (icellX1*ncells.height + icellY1)*nbins;
                    471	                    data->histWeights[1] = cellX*cellY;
                    472	                    data->histOfs[2] = data->histOfs[3] = 0;
                    473	                    data->histWeights[2] = data->histWeights[3] = 0;
                    474	                }
                475	                else
                  476	                {
                    477	                    data = &pixData[count1++];
                    478	                    if( (unsigned)icellY0 < (unsigned)ncells.height )
                      479	                    {
                        480	                        icellY1 = icellY0;
                        481	                        cellY = 1.f - cellY;
                        482	                    }
                    483	                    data->histOfs[0] = (icellX1*ncells.height + icellY1)*nbins;
                    484	                    data->histWeights[0] = cellX*cellY;
                    485	                    data->histOfs[1] = data->histOfs[2] = data->histOfs[3] = 0;
                    486	                    data->histWeights[1] = data->histWeights[2] = data->histWeights[3] = 0;
                    487	                }
                488	            }
            489	            data->gradOfs = (grad.cols*i + j)*2;
            490	            data->qangleOfs = (qangle.cols*i + j)*2;
            491	            data->gradWeight = weights(i,j);
            492	        }
      493
      494	    assert( count1 + count2 + count4 == rawBlockSize );
      495	    // defragment pixData
      496	    for( j = 0; j < count2; j++ )
        497	        pixData[j + count1] = pixData[j + rawBlockSize];
      498	    for( j = 0; j < count4; j++ )
        499	        pixData[j + count1 + count2] = pixData[j + rawBlockSize*2];
      500	    count2 += count1;
      501	    count4 += count2;
      502
      503	    // initialize blockData
      504	    for( j = 0; j < nblocks.width; j++ )
        505	        for( i = 0; i < nblocks.height; i++ )
          506	        {
            507	            BlockData& data = blockData[j*nblocks.height + i];
            508	            data.histOfs = (j*nblocks.height + i)*blockHistogramSize;
            509	            data.imgOffset = Point(j*blockStride.width,i*blockStride.height);
            510	        }
      511	}
    512
    513
    514	const float* HOGCache::getBlock(Point pt, float* buf)
    515	{
      516	    float* blockHist = buf;
      517	    assert(descriptor != 0);
      518
      519	    Size blockSize = descriptor->blockSize;
      520	    pt += imgoffset;
      521
      522	    CV_Assert( (unsigned)pt.x <= (unsigned)(grad.cols - blockSize.width) &&
                        523	               (unsigned)pt.y <= (unsigned)(grad.rows - blockSize.height) );
      524
      525	    if( useCache )
        526	    {
          527	        CV_Assert( pt.x % cacheStride.width == 0 &&
                                528	                   pt.y % cacheStride.height == 0 );
          529	        Point cacheIdx(pt.x/cacheStride.width,
                                     530	                      (pt.y/cacheStride.height) % blockCache.rows);
          531	        if( pt.y != ymaxCached[cacheIdx.y] )
            532	        {
              533	            Mat_<uchar> cacheRow = blockCacheFlags.row(cacheIdx.y);
              534	            cacheRow = (uchar)0;
              535	            ymaxCached[cacheIdx.y] = pt.y;
              536	        }
          537
          538	        blockHist = &blockCache[cacheIdx.y][cacheIdx.x*blockHistogramSize];
          539	        uchar& computedFlag = blockCacheFlags(cacheIdx.y, cacheIdx.x);
          540	        if( computedFlag != 0 )
            541	            return blockHist;
          542	        computedFlag = (uchar)1; // set it at once, before actual computing
          543	    }
      544
      545	    int k, C1 = count1, C2 = count2, C4 = count4;
      546	    const float* gradPtr = (const float*)(grad.data + grad.step*pt.y) + pt.x*2;
      547	    const uchar* qanglePtr = qangle.data + qangle.step*pt.y + pt.x*2;
      548
      549	    CV_Assert( blockHist != 0 );
      550
      551	    for( k = 0; k < blockHistogramSize; k++ )
        552	        blockHist[k] = 0.f;
      553
      554	    const PixData* _pixData = &pixData[0];
      555
      556	    for( k = 0; k < C1; k++ )
        557	    {
          558	        const PixData& pk = _pixData[k];
          559	        const float* a = gradPtr + pk.gradOfs;
          560	        float w = pk.gradWeight*pk.histWeights[0];
          561	        const uchar* h = qanglePtr + pk.qangleOfs;
          562	        int h0 = h[0], h1 = h[1];
          563	        float* hist = blockHist + pk.histOfs[0];
          564	        float t0 = hist[h0] + a[0]*w;
          565	        float t1 = hist[h1] + a[1]*w;
          566	        hist[h0] = t0; hist[h1] = t1;
          567	    }
      568
      569	    for( ; k < C2; k++ )
        570	    {
          571	        const PixData& pk = _pixData[k];
          572	        const float* a = gradPtr + pk.gradOfs;
          573	        float w, t0, t1, a0 = a[0], a1 = a[1];
          574	        const uchar* h = qanglePtr + pk.qangleOfs;
          575	        int h0 = h[0], h1 = h[1];
          576
          577	        float* hist = blockHist + pk.histOfs[0];
          578	        w = pk.gradWeight*pk.histWeights[0];
          579	        t0 = hist[h0] + a0*w;
          580	        t1 = hist[h1] + a1*w;
          581	        hist[h0] = t0; hist[h1] = t1;
          582
          583	        hist = blockHist + pk.histOfs[1];
          584	        w = pk.gradWeight*pk.histWeights[1];
          585	        t0 = hist[h0] + a0*w;
          586	        t1 = hist[h1] + a1*w;
          587	        hist[h0] = t0; hist[h1] = t1;
          588	    }
      589
      590	    for( ; k < C4; k++ )
        591	    {
          592	        const PixData& pk = _pixData[k];
          593	        const float* a = gradPtr + pk.gradOfs;
          594	        float w, t0, t1, a0 = a[0], a1 = a[1];
          595	        const uchar* h = qanglePtr + pk.qangleOfs;
          596	        int h0 = h[0], h1 = h[1];
          597
          598	        float* hist = blockHist + pk.histOfs[0];
          599	        w = pk.gradWeight*pk.histWeights[0];
          600	        t0 = hist[h0] + a0*w;
          601	        t1 = hist[h1] + a1*w;
          602	        hist[h0] = t0; hist[h1] = t1;
          603
          604	        hist = blockHist + pk.histOfs[1];
          605	        w = pk.gradWeight*pk.histWeights[1];
          606	        t0 = hist[h0] + a0*w;
          607	        t1 = hist[h1] + a1*w;
          608	        hist[h0] = t0; hist[h1] = t1;
          609
          610	        hist = blockHist + pk.histOfs[2];
          611	        w = pk.gradWeight*pk.histWeights[2];
          612	        t0 = hist[h0] + a0*w;
          613	        t1 = hist[h1] + a1*w;
          614	        hist[h0] = t0; hist[h1] = t1;
          615
          616	        hist = blockHist + pk.histOfs[3];
          617	        w = pk.gradWeight*pk.histWeights[3];
          618	        t0 = hist[h0] + a0*w;
          619	        t1 = hist[h1] + a1*w;
          620	        hist[h0] = t0; hist[h1] = t1;
          621	    }
      622
      623	    normalizeBlockHistogram(blockHist);
      624
      625	    return blockHist;
      626	}
    627
    628
    629	void HOGCache::normalizeBlockHistogram(float* _hist) const
    630	{
      631	    float* hist = &_hist[0];
      632	    size_t i, sz = blockHistogramSize;
      633
      634	    float sum = 0;
      635	    for( i = 0; i < sz; i++ )
        636	        sum += hist[i]*hist[i];
      637	    float scale = 1.f/(std::sqrt(sum)+sz*0.1f), thresh = (float)descriptor->L2HysThreshold;
      638	    for( i = 0, sum = 0; i < sz; i++ )
        639	    {
          640	        hist[i] = std::min(hist[i]*scale, thresh);
          641	        sum += hist[i]*hist[i];
          642	    }
      643	    scale = 1.f/(std::sqrt(sum)+1e-3f);
      644	    for( i = 0; i < sz; i++ )
        645	        hist[i] *= scale;
      646	}
    647
    648
    649	Size HOGCache::windowsInImage(Size imageSize, Size winStride) const
    650	{
      651	    return Size((imageSize.width - winSize.width)/winStride.width + 1,
                          652	                (imageSize.height - winSize.height)/winStride.height + 1);
      653	}
    654
    655	Rect HOGCache::getWindow(Size imageSize, Size winStride, int idx) const
    656	{
      657	    int nwindowsX = (imageSize.width - winSize.width)/winStride.width + 1;
      658	    int y = idx / nwindowsX;
      659	    int x = idx - nwindowsX*y;
      660	    return Rect( x*winStride.width, y*winStride.height, winSize.width, winSize.height );
      661	}
    662
    663
    664	void HOGDescriptor::compute(const Mat& img, vector<float>& descriptors,
                                    665	                            Size winStride, Size padding,
                                    666	                            const vector<Point>& locations) const
    667	{
      668	    if( winStride == Size() )
        669	        winStride = cellSize;
      670	    Size cacheStride(gcd(winStride.width, blockStride.width),
                               671	                     gcd(winStride.height, blockStride.height));
      672	    size_t nwindows = locations.size();
      673	    padding.width = (int)alignSize(std::max(padding.width, 0), cacheStride.width);
      674	    padding.height = (int)alignSize(std::max(padding.height, 0), cacheStride.height);
      675	    Size paddedImgSize(img.cols + padding.width*2, img.rows + padding.height*2);
      676
      677	    HOGCache cache(this, img, padding, padding, nwindows == 0, cacheStride);
      678
      679	    if( !nwindows )
        680	        nwindows = cache.windowsInImage(paddedImgSize, winStride).area();
      681
      682	    const HOGCache::BlockData* blockData = &cache.blockData[0];
      683
      684	    int nblocks = cache.nblocks.area();
      685	    int blockHistogramSize = cache.blockHistogramSize;
      686	    size_t dsize = getDescriptorSize();
      687	    descriptors.resize(dsize*nwindows);
      688
      689	    for( size_t i = 0; i < nwindows; i++ )
        690	    {
          691	        float* descriptor = &descriptors[i*dsize];
          692
          693	        Point pt0;
          694	        if( !locations.empty() )
            695	        {
              696	            pt0 = locations[i];
              697	            if( pt0.x < -padding.width || pt0.x > img.cols + padding.width - winSize.width ||
                                 698	                pt0.y < -padding.height || pt0.y > img.rows + padding.height - winSize.height )
                699	                continue;
              700	        }
          701	        else
            702	        {
              703	            pt0 = cache.getWindow(paddedImgSize, winStride, (int)i).tl() - Point(padding);
              704	            CV_Assert(pt0.x % cacheStride.width == 0 && pt0.y % cacheStride.height == 0);
              705	        }
          706
          707	        for( int j = 0; j < nblocks; j++ )
            708	        {
              709	            const HOGCache::BlockData& bj = blockData[j];
              710	            Point pt = pt0 + bj.imgOffset;
              711
              712	            float* dst = descriptor + bj.histOfs;
              713	            const float* src = cache.getBlock(pt, dst);
              714	            if( src != dst )
                715	                for( int k = 0; k < blockHistogramSize; k++ )
                  716	                    dst[k] = src[k];
              717	        }
          718	    }
      719	}
    720
    721
    722	void HOGDescriptor::detect(const Mat& img,
                                   723	    vector<Point>& hits, double hitThreshold,
                                   724	    Size winStride, Size padding, const vector<Point>& locations) const
    725	{
      726	    hits.clear();
      727	    if( svmDetector.empty() )
        728	        return;
      729
      730	    if( winStride == Size() )
        731	        winStride = cellSize;
      732	    Size cacheStride(gcd(winStride.width, blockStride.width),
                               733	                     gcd(winStride.height, blockStride.height));
      734	    size_t nwindows = locations.size();
      735	    padding.width = (int)alignSize(std::max(padding.width, 0), cacheStride.width);
      736	    padding.height = (int)alignSize(std::max(padding.height, 0), cacheStride.height);
      737	    Size paddedImgSize(img.cols + padding.width*2, img.rows + padding.height*2);
      738
      739	    HOGCache cache(this, img, padding, padding, nwindows == 0, cacheStride);
      740
      741	    if( !nwindows )
        742	        nwindows = cache.windowsInImage(paddedImgSize, winStride).area();
      743
      744	    const HOGCache::BlockData* blockData = &cache.blockData[0];
      745
      746	    int nblocks = cache.nblocks.area();
      747	    int blockHistogramSize = cache.blockHistogramSize;
      748	    size_t dsize = getDescriptorSize();
      749
      750	    double rho = svmDetector.size() > dsize ? svmDetector[dsize] : 0;
      751	    vector<float> blockHist(blockHistogramSize);
      752
      753	    for( size_t i = 0; i < nwindows; i++ )
        754	    {
          755	        Point pt0;
          756	        if( !locations.empty() )
            757	        {
              758	            pt0 = locations[i];
              759	            if( pt0.x < -padding.width || pt0.x > img.cols + padding.width - winSize.width ||
                                 760	                pt0.y < -padding.height || pt0.y > img.rows + padding.height - winSize.height )
                761	                continue;
              762	        }
          763	        else
            764	        {
              765	            pt0 = cache.getWindow(paddedImgSize, winStride, (int)i).tl() - Point(padding);
              766	            CV_Assert(pt0.x % cacheStride.width == 0 && pt0.y % cacheStride.height == 0);
              767	        }
          768	        double s = rho;
          769	        const float* svmVec = &svmDetector[0];
          770	        int j, k;
          771	        for( j = 0; j < nblocks; j++, svmVec += blockHistogramSize )
            772	        {
              773	            const HOGCache::BlockData& bj = blockData[j];
              774	            Point pt = pt0 + bj.imgOffset;
              775
              776	            const float* vec = cache.getBlock(pt, &blockHist[0]);
              777	            for( k = 0; k <= blockHistogramSize - 4; k += 4 )
                778	                s += vec[k]*svmVec[k] + vec[k+1]*svmVec[k+1] +
                779	                    vec[k+2]*svmVec[k+2] + vec[k+3]*svmVec[k+3];
              780	            for( ; k < blockHistogramSize; k++ )
                781	                s += vec[k]*svmVec[k];
              782	        }
          783	        if( s >= hitThreshold )
            784	            hits.push_back(pt0);
          785	    }
      786	}
    787
    788
    789	struct HOGInvoker
    790	{
      791	    HOGInvoker( const HOGDescriptor* _hog, const Mat& _img,
                         792	                double _hitThreshold, Size _winStride, Size _padding,
                         793	                const double* _levelScale, ConcurrentRectVector* _vec )
      794	    {
        795	        hog = _hog;
        796	        img = _img;
        797	        hitThreshold = _hitThreshold;
        798	        winStride = _winStride;
        799	        padding = _padding;
        800	        levelScale = _levelScale;
        801	        vec = _vec;
        802	    }
      803
      804	    void operator()( const BlockedRange& range ) const
      805	    {
        806	        int i, i1 = range.begin(), i2 = range.end();
        807	        double minScale = i1 > 0 ? levelScale[i1] : i2 > 1 ? levelScale[i1+1] : std::max(img.cols, img.rows);
        808	        Size maxSz(cvCeil(img.cols/minScale), cvCeil(img.rows/minScale));
        809	        Mat smallerImgBuf(maxSz, img.type());
        810	        vector<Point> locations;
        811
        812	        for( i = i1; i < i2; i++ )
          813	        {
            814	            double scale = levelScale[i];
            815	            Size sz(cvRound(img.cols/scale), cvRound(img.rows/scale));
            816	            Mat smallerImg(sz, img.type(), smallerImgBuf.data);
            817	            if( sz == img.size() )
              818	                smallerImg = Mat(sz, img.type(), img.data, img.step);
            819	            else
              820	                resize(img, smallerImg, sz);
            821	            hog->detect(smallerImg, locations, hitThreshold, winStride, padding);
            822	            Size scaledWinSize = Size(cvRound(hog->winSize.width*scale), cvRound(hog->winSize.height*scale));
            823	            for( size_t j = 0; j < locations.size(); j++ )
              824	                vec->push_back(Rect(cvRound(locations[j].x*scale),
                                                      825	                                    cvRound(locations[j].y*scale),
                                                      826	                                    scaledWinSize.width, scaledWinSize.height));
            827	        }
        828	    }
      829
      830	    const HOGDescriptor* hog;
      831	    Mat img;
      832	    double hitThreshold;
      833	    Size winStride;
      834	    Size padding;
      835	    const double* levelScale;
      836	    ConcurrentRectVector* vec;
      837	};
    838
    839
    840	void HOGDescriptor::detectMultiScale(
                                             841	    const Mat& img, vector<Rect>& foundLocations,
                                             842	    double hitThreshold, Size winStride, Size padding,
                                             843	    double scale0, int groupThreshold) const
    844	{
      845	    double scale = 1.;
      846	    const int maxLevels = 64;
      847	    int levels = 0;
      848
      849	    vector<double> levelScale;
      850	    for( levels = 0; levels < maxLevels; levels++ )
        851	    {
          852	        levelScale.push_back(scale);
          853	        if( cvRound(img.cols/scale) < winSize.width ||
                         854	            cvRound(img.rows/scale) < winSize.height ||
                         855	            scale0 <= 1 )
            856	            break;
          857	        scale *= scale0;
          858	    }
      859	    levels = std::max(levels, 1);
      860	    levelScale.resize(levels);
      861
      862	    ConcurrentRectVector allCandidates;
      863
      864	    parallel_for(BlockedRange(0, (int)levelScale.size()),
                           865	                 HOGInvoker(this, img, hitThreshold, winStride, padding, &levelScale[0], &allCandidates));
      866
      867	    foundLocations.resize(allCandidates.size());
      868	    std::copy(allCandidates.begin(), allCandidates.end(), foundLocations.begin());
      869
      870	    groupRectangles(foundLocations, groupThreshold, 0.2);
      871	}
*/