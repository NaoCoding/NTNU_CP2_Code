YouBike Station Search App

Requirements:
    curl , jq

Features:
    Search YouBike Stations by UID
    Search YouBike Stations by English/Chinese 
        - substring supported
        - lower/upper characters supported
    Show informations of each YouBike Stations in Taipei
        - Chinese station name supported


Usage: ./hw0402 [options]
    -h , --help : display help message
    -u , --uid : search YouBike station by UID (optarg required)
    -e , --en : search YouBike station by English Name(optarg required)
    -c , --cn : search YouBike station by Chinese Name(optarg required)

使用方法 : ./hw0402 [ 選項 ]
    -h , --help : 顯示說明/幫助訊息
    -u , --uid : 使用UID搜尋YouBike站牌 (參數必填)
    -e , --en : 使用英文站名搜尋YouBike站牌 (參數必填)
    -c , --cn : 使用中文站名搜尋YouBike站牌 (參數必填)