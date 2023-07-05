## 1. 程式功能  
 - 讀取TXT格式迷宮資料。  
 - 找出左上到右下最短路徑。  
 - 圖形繪製迷宮。  
 - 可編輯迷宮、讀取與儲存迷宮，改變起點、終點位置。  
左鍵放置障礙、右鍵為清除。按下左Alt時左鍵則為設置終點、右鍵為設置起點。  
## 2. 設計方法  
製作3專案、依序分別為1_MazeShortestPathGLM、2_MazeShortestPath、  3_MazeShortestPathGL。其中2_MazeShortes為本次題目要求所需，其餘均有額外功能且不一定完全符合題目輸出格式。  
### MazeShortestPathGLM  
1. 使用C++內建的Vector、Queue函數庫與GLM(OpenGL Mathematics)快速完成所需功能。  
2. 將Maze.txt所得的2維陣列，將其長寬兩側均Padding 1，使計算時直接判端是否有障礙，而無須判斷是否超出陣列範圍。  
3. 使用BFS(Breadth-First Search)，每次一移動時均記錄距離，直到到達終點。  
4. 到終點後，從終點開始依據鄰近點最小距離找出路徑。  
### MazeShortestPath  
1. 將Queue以前次作業製作的Lis取代。Vector使用自行動態分配陣列取代。GLM則不使用，於函數內自行計算。  
### MazeShortestPathGL  
1. 用以練習Geometry Shader的應用。使用OpenGL 4.6繪製迷宮。  
2. GUI 使用Dear ImGui與ImGuiFileDialog製作。  
## 3. 問題討論  
### Geometry Shader詳細運作方式仍不清楚。  
Geometry Shader 多繪製出最左上角1點。  
### 應能縮短繪製新路經時間  
在迷宮改變時均會繪製新路徑，且路徑更新方式為直接建立新的Vertex Array Object。應能與Texture相似，只透過修改Vertex Array Buffer方式更新路徑，省去建立新物件。  
## 3. 結果 
| <img src="https://i.imgur.com/lGFDG0t.png" width="300" height="300" /> | <img src="https://i.imgur.com/JO02TAv.png" width="300" height="300" /> | <img src="https://i.imgur.com/KlYW0bZ.png" width="300" height="300" /> |
| :-: | :-: | :-: |
| 圖1 於1_MazeShortestPathGLM使用與範例相同順序與資料插入結果 | 圖2 於2_MazeShortestPath使用與範例相同順序與資料插入結果 | 圖3 3_MazeShortestPathGL繪製結果 |