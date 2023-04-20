# Student Data Management
## 目的  
使用C++中Structure與陣列儲存資料並排序。  
## 功能  
1. 預設讀取同路徑下"list2.txt"檔案，或於Terminal中以argument指定開啟檔案。  
2. 依據平均分數由小至大於Terminal中顯示學生資訊，並另外顯示最高分學生資訊。  
## 方法  
1. 因目前檔案大小較小，一次將所有檔案讀出並關閉以便其他程式使用。故在函數GetFileContents()中一次讀出所有資料並關閉檔案。在資料量較大時應於所需資料時在到硬碟中獲取。  
2. 使用C的printf相較C++的cout較為簡短且較容易控制輸出字串格式。但Visual Studio可能因記憶體安全性問題報錯並無法編譯，需定義_CRT_SECURE_NO_WARNINGS避免錯誤。  
3. 由於一學生資料較多，因此在排序前先建立一indices陣列指向每一筆學生資料。此時在排序時只對indices排序減少資料搬移次數。
4.   排序使用泡沫排序(Bubble Sort)。  
## 討論
Structure所使用實際記憶體空間。假設使用一64位元電腦，一指標需8Bytes(64Bits)。必有1名字且至少1字元。personalID為8字元，studentID為9字元，科系(department)為2字元。  
1. 將不確定大小的學生名字(Name)使用動態分配方式儲存，故至少需記錄一指標(8Bytes)與至少一字之字串(>=2Bytes)。最少需使用10Bytes。  
2. 將personalID和studentID均為固定大小，包含字母與數字，為便於處理，使用字串方式儲存，故多使用一字元紀錄’\0’，最少需使用(8+1)+(9+1) = 19Bytes。  
3. 年齡(age)因一般均無法超過255，故使用1Byte。  
4. 科系(department)一般均也不超過255，故使用1Byte紀錄編號，若須科系名稱在使用一陣列查表取得。  
5. scores由於經常會有非整數資料，故以Float(4Bytes)表示。三筆分數需使用4*3=12Bytes。若分數只限0~100整數則可只使用1Byte紀錄，三筆分數只需使用3Bytes。  

依據1~5點一學生最少須使用43Bytes(使用Float記錄分數)，34Bytes(使用uint8_t記錄分數)。但由於編譯過程中會對Struct中資料做對齊(alignment)，跳過些許Bytes，故不一定能達到最佳43Bytes。以此題為例此結構實際使用41Bytes(不包含名字指標所指向字串)，但若於程式中使用sizeof()指令會發現其使用了48Bytes(不同環境可能不同)。

編譯後結構儲存方式如下( 使用程式中PrintStructureSize()函式 )
```
struct Student (48 Bytes)
   |Addr|-   VarName (  Size  )
   |0000|-      name ( 8 Bytes)
   |0008|-personalID ( 9 Bytes)
   |0017|- studentID (10 Bytes)
   |0027|-       age ( 1 Bytes)
   |0028|-department ( 1 Bytes)
   |0032|-    scores (12 Bytes)

```

## 結果  
[]()