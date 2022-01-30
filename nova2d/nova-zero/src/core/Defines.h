#pragma once

namespace novazero
{
	namespace core
	{
		// Max gamepads
		#define MAX_JOYSTICKS 8

		// Max draw layers
		#define MAX_LAYERS 20

		// Max graver groups
		#define MAX_GRAVER_GROUPS 255

	}
}

/*
nova2d Get Unique Game ID
Returns a unique unsigned int ID value
*/
#define n2dGameGetID() novazero::core::Game::GetNextID();

/*
nova2d Get Delta Time
Returns time between frames
*/
#define n2dDeltaTime() novazero::core::Game::GetDeltaTime()

/*
nova2d Get Time Scale
Returns a float value which directly effects all moving objects
*/
#define n2dTimeScale novazero::core::Game::s_TimeScale

/*
nova2d Set Time Scale (float value)
Sets a float value which directly effects all moving objects
*/
#define n2dTimeScaleSet(timeScale) novazero::core::Game::s_TimeScale = timeScale;

/*
nova2d Random Float (float min, float max)
Returns a random float in range
*/
#define n2dRandomFloat(min, max) novazero::core::Game::RandomFloat(min, max);

/*
nova2d Random Int (int min, int max)
Returns a random integer in range
*/
#define n2dRandomInt(min, max) novazero::core::Game::RandomInt(min, max);

/*
nova2d Round To Even (number)
*/
#define n2dRoundEven(num) (num % 2 == 0) ? num : (num + 1);

/*
nova2d Get total instances
Returns total drawables on screen
*/
#define n2dDebugDrawableCount() novazero::core::Game::s_Renderer->s_DrawLayers->GetInstanceCount();

/*
nova2d Get total updaters
Returns total updaters in Game processor
*/
#define n2dDebugUpdaterCount() novazero::core::SceneManager::GetUpdaterCount();

/*
nova2d Get total deleteables
Returns total deleteables (similar to drawables)
*/
#define n2dDebugDeleteableCount() novazero::core::SceneManager::GetDeleteableCount();

/*
nova2d Get total time effectors
Returns total time effectors (similar to gravers)
*/
#define n2dDebugTimeEffectorsCount() novazero::core::Game::s_SceneManager->s_TimeEffectorManager->GetEffectorCount();

/*
nova2d Get total time effecteds
Returns total time effected objects (similar to effectors)
*/
#define n2dDebugTimeEffectedCount() novazero::core::Game::s_SceneManager->s_TimeEffectorManager->GetEffectedCount();

/*
nova2d Get total referenced
Returns total referenced objs (similar to deletables)
*/
#define n2dDebugReferencedCount() novazero::core::Game::s_SceneManager->s_ReferenceManager->GetReferenceCount();

/*
nova2d Get total gravers
Returns total graver objs (similar to colliders)
*/
#define n2dDebugGraverCount() novazero::core::Game::s_SceneManager->s_GraverManager->GetGraverCount();

/*
nova2d Get total tweens
Returns total tweens (usually attached to positionals)
*/
#define n2dDebugTweenCount() novazero::core::Game::s_SceneManager->s_TweenManager->GetTweenCount();

/*
nova2d Add Tween Int
Tweens *refInt (referenced int) from start to end in duration milliseconds
*/
#define n2dTweenAddInt(refInt, start, end, durationMS) novazero::core::Game::s_SceneManager->s_TweenManager->AddTweenInt(refInt, start, end, durationMS);

/*
nova2d Add Tween Float
Tweens *refFloat (referenced float) from start to end in duration milliseconds
*/
#define n2dTweenAddFloat(refFloat, start, end, durationMS) novazero::core::Game::s_SceneManager->s_TweenManager->AddTweenFloat(refFloat, start, end, durationMS);

/*
nova2d SQL Configure(std::string connectionString, std::string table, std::string user, std::string pass)
Enables SQL use
*/
#define n2dSQLConfigure(connectionString, table, user, pass) novazero::core::Game::s_SQLManager->Configure(connectionString, table, user, pass);

/*
nova2d SQL Scores Create Table (int, int)
Creates simple nova2d highscores table
*/
#define n2dSQLScoreTableCreation(nameCharCount, scoreDigitCount) novazero::core::Game::s_SQLManager->CreateScoreTable(nameCharCount, scoreDigitCount);

/*
nova2d SQL Scores Add (string int)
Inserts new score into nova2d's simple highscore table
*/
#define n2dSQLScoreAdd(playerName, score) novazero::core::Game::s_SQLManager->AddScore(playerName, score);

/*
nova2d SQL Scores Remove (string, int)
Removes score from nova2d's simple highscore table
*/
#define n2dSQLScoreRemove(playerName, score) novazero::core::Game::s_SQLManager->RemoveScore(playerName, score);

/*
nova2d SQL Get Scores (count, &results, &lowestScore)
Grabs top N amount of scores
*/
#define n2dSQLScoreTopN(count, results, lowest) novazero::core::Game::s_SQLManager->GetScores(count, results, lowest);

/*
nova2d Clear Text edit buffer (limitSizeOfBuffer)
Clears textbox input buffer
*/
#define n2dTextInputClearBuffer(newBufferMax) novazero::core::Game::s_InputHandler->CleanTextBuffer(newBufferMax);

/*
nova2d Set Text edit buffer (limitSizeOfBuffer)
Sets text in input buffer
*/
#define n2dTextInputSetBuffer(fillText, maxChars) novazero::core::Game::s_InputHandler->SetTextBuffer(fillText, maxChars);

/*
nova2d Add Key Down Listener (SDL_KeyCode key, void() function, context)
Calls function on SDL_KeyCode key down
Must be called on in a class that inherits from EventListener
*/
#define n2dAddKeyDownListener(key, func, context) AddKeysEventListener(key, &InputHandler::IsKeyDown, std::bind(&func, context));

/*
nova2d Remove Key Down Listener (SDL_KeyCode key)
Removes previously added key down listener attached to SLD_KeyCode key
Must be called on in a class that inherits from EventListener
*/
#define n2dRemoveKeyDownListener(key) RemoveEventListener(key);

/*
nova2d Add Updater(void(*f) funcToCall, context)
Updaters are called each frame; this adds a new funcToCall as an Updater
RETURNS unsigned int CleanID
*/
#define n2dAddUpdater(funcToCall, context) novazero::core::SceneManager::AddUpdater(std::bind(&funcToCall, context));

/*
nova2d Remove Updater(void(*f) funcToRemove)
Updaters are called each frame; this removes a current updater function from updaters vector<void()>
*/
#define n2dRemoveUpdater(id) novazero::core::SceneManager::RemoveUpdater(id);

/*
nova2d Add Scene (std::string sceneName, Scene* scene)
Scenes are levels, menus, etc; this adds a scene to Game's Scene Manager
*/
#define n2dGameAddScene(scene) novazero::core::Game::s_SceneManager->AddScene(scene->m_SceneName, scene);

/*
nova2d Configure first game scene to start with (std::string sceneName)
Scenes are levels, menus, etc; this adds a scene to Game's Scene Manager
*/
#define n2dGameConfigFirstScene(scene) novazero::core::Game::s_SceneManager->ConfigureFirstScene(scene->m_SceneName);

/*
nova2d Load and Add Texture (std::string assetName, std::string resourcePath)
Loads asset into Game's Asset Manager
*/
#define n2dAssetsLoadAndAddTexture(assetName, assetPath) novazero::core::Game::s_AssetManager->LoadAndAddTexture(assetName, assetPath);

/*
nova2d Add Color (std::string colorName, std::string colorHex, float alpha)
Adds Color to Game's Color Manager. ex: n2dAddColor("white", "ffffff", 1)
*/
#define n2dAddColor(colorName, hexCode, alpha) novazero::core::Game::s_ColorManager->AddColor(colorName, hexCode, alpha);

/*
nova2d Get Color (std::string colorName)
Gets Color from Game's Color Manager
*/
#define n2dGetColor(colorName) novazero::core::Game::s_ColorManager->GetColor(colorName);

/*
nova2d SetBackgroundColor(std::string colorName)
Sets default background to a color already loaded in Game's Color Manager
*/
#define n2dSetBackgroundColor(colorName) novazero::core::Game::s_Renderer->SetBackgroundColor(colorName);

/*
nova2d Change Scene(std::string sceneName)
Changes to Game's Scene Manager preloaded scene
*/
#define n2dSceneChange(sceneName) novazero::core::Game::s_SceneManager->ChangeScene(sceneName);

/*
nova2d Set draw color (std::string colorName)
Updates SDL renderer's draw color from preloaded Game's Color Manager's color
*/
#define n2dRenderDrawColor(colorName) novazero::core::Game::s_Renderer->SetDrawColor(colorName);

/*
nova2d Add deleteable
Adds a Deleteable* to Game's Scene Manager's deleteable tracker
RETURNS an unsigned int ID of deleteable for cleanup
*/
#define n2dAddDeleteable(deleteable) novazero::core::Game::s_SceneManager->AddDeleteable(deleteable);

/*
nova2d Remove deleteable
Removes a Deleteable* to Game's Scene Manager's deleteable tracker
*/
#define n2dRemoveDeleteable(id) novazero::core::Game::s_SceneManager->RemoveDeleteable(id);

/*
nova2d Add Drawable (Drawable* drawable, char layer)
Adds a graphic/drawable to Game's Draw Layers
RETURNS: void
*/
#define n2dAddDrawable(drawable, layer) novazero::core::Game::s_Renderer->s_DrawLayers->AddSprite(drawable, layer);

/*
nova2d Remove Drawable (Drawable* drawable, char layer)
Removes a graphic/drawable from Game's Draw Layers
*/
#define n2dRemoveDrawable(drawableID, layer) novazero::core::Game::s_Renderer->s_DrawLayers->RemoveSprite(drawableID, layer)

/*
nova2d Add Reference (std::string referenceName, Referenceable* reference)
Adds a reference to Referenceable object into Game's Reference Manager
*/
#define n2dReferenceAdd(referenceName, reference) novazero::core::SceneManager::s_ReferenceManager->AddReference(referenceName, reference);

/*
nova2d Get Reference (std::string referenceName)
Gets a reference to Referenceable object from Game's Reference Manager
*/
#define n2dReferenceGet(referenceName) novazero::core::SceneManager::s_ReferenceManager->GetReferenced(referenceName);

/*
nova2d Remove Reference (std::string referenceName)
Removes a reference from Game's Reference Manager
*/
#define n2dReferenceRemove(referenceName) novazero::core::SceneManager::s_ReferenceManager->RemoveReference(referenceName);

/*
nova2d Add Font (std::string fontName, std::string fontPath, int fontSize)
Adds font to Game's Font Manager
*/
#define n2dFontAdd(fontName, fontPath, fontSize) novazero::core::Game::s_FontManager->AddFont(fontName, fontPath, fontSize);

/*
nova2d Get Font (std::string fontName)
Adds font to Game's Font Manager
*/
#define n2dFontGet(fontName) novazero::core::Game::s_FontManager->GetFont(fontName);

/*
nova2d Global Game Score ()
Easy access globally to game score
*/
#define n2dScoreSet(score) novazero::core::Game::SetScore(score);

/*
nova2d Global Game Score ()
Easy access globally to game score
*/
#define n2dScoreGet() novazero::core::Game::GetScore();

/*
nova2d Global Game Score ()
Easy access globally to game score
*/
#define n2dScoreAdd(add) novazero::core::Game::AddScore(add);