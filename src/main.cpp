#include "main.hpp"
#include "MainConfig.hpp"
using namespace SaberTweaks;
using namespace UnityEngine;
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/LineRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/SaberBurnMarkArea.hpp"
#include "questui/shared/QuestUI.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"
static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
DEFINE_CONFIG(MainConfig);
// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

DEFINE_TYPE(SaberTweaks, MainUI);


GameObject *mainCont;
GameObject *l_saberColor;
GameObject *r_saberColor;

bool screensEnabled = false;

void ChangeOverideColor(bool newval)
{
    getMainConfig().OverideColors.SetValue(newval);
    l_saberColor->get_gameObject()->SetActive(newval);
    r_saberColor->get_gameObject()->SetActive(newval);
}
void ChangeLifeTime(bool newval)
{
    getMainConfig().OverideColors.SetValue(newval);
    l_saberColor->get_gameObject()->SetActive(newval);
    r_saberColor->get_gameObject()->SetActive(newval);
}
void OnChangeScale (float newval)
{
    newval = std::round(newval * 100) / 100;
    getMainConfig().BurnScale.SetValue(newval);
}
void OnChangeLifetime (float newval)
{
    newval = std::round(newval * 100) / 100;
    getMainConfig().BurnLifeTime.SetValue(newval);
}
void OnChangeIntensity (float newval)
{
    newval = std::round(newval * 100) / 100;
    getMainConfig().BurnOpacity.SetValue(newval);
}
void SaberTweaks::MainUI::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    using namespace QuestUI;
    if (firstActivation)
    {
        mainCont = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        l_saberColor = QuestUI::BeatSaberUI::CreateFloatingScreen(Vector2(85.0f, 0.f), Vector3(-3.5f, 1.5f, 3.5f), Vector3(0.0f, -35.0f, 0.0f), 0.0f, true, false);
        r_saberColor = QuestUI::BeatSaberUI::CreateFloatingScreen(Vector2(85.0f, 0.f), Vector3(-3.5f, 1.0f, 3.5f), Vector3(0.0f, -35.0f, 0.0f), 0.0f, true, false);

        //Toggle
        AddConfigValueToggle(mainCont->get_transform(), getMainConfig().ModEnabled);
        QuestUI::BeatSaberUI::CreateToggle(mainCont->get_transform(), "Override Colors", getMainConfig().OverideColors.GetValue(), ChangeOverideColor);
        //QuestUI::BeatSaberUI::CreateToggle(mainCont->get_transform(), "Never Fade", getMainConfig().OverideLifetime.GetValue(), ChangeLifeTime);
        QuestUI::BeatSaberUI::CreateIncrementSetting(mainCont->get_transform(), "Scale", 2, 0.1f, getMainConfig().BurnScale.GetValue(), 0.2f, 3.0f, OnChangeScale);
        QuestUI::BeatSaberUI::CreateIncrementSetting(mainCont->get_transform(), "Burn Lifetime", 2, 0.5f, getMainConfig().BurnLifeTime.GetValue(), 0.0f, 10.0f, OnChangeLifetime);
        QuestUI::BeatSaberUI::CreateIncrementSetting(mainCont->get_transform(), "Intensity", 2, 0.2f, getMainConfig().BurnOpacity.GetValue(), 0.2f, 1.0f, OnChangeIntensity);

        auto l_colorPicker = BeatSaberUI::CreateColorPicker(l_saberColor->get_transform(), "Left Burn Mark Color", getMainConfig().LeftSaberColor.GetValue(),[](UnityEngine::Color color) {
            getMainConfig().LeftSaberColor.SetValue(color, true);
        });

        QuestUI::BeatSaberUI::CreateUIButton(l_colorPicker->get_transform(), "Refresh",UnityEngine::Vector2(0,+0.3),UnityEngine::Vector2(30,8),
        [l_colorPicker]()
            {
                getMainConfig().LeftSaberColor.SetValue(UnityEngine::Color(5.0, 5.0, 5.0, 5.0));
                l_colorPicker->set_currentColor(getMainConfig().LeftSaberColor.GetValue());
            });

        auto r_colorPicker = BeatSaberUI::CreateColorPicker(r_saberColor->get_transform(), "Right Burn Mark Color", getMainConfig().RightSaberColor.GetValue(),[](UnityEngine::Color color) {
            getMainConfig().RightSaberColor.SetValue(color, true);
        });

        QuestUI::BeatSaberUI::CreateUIButton(r_colorPicker->get_transform(), "Refresh",UnityEngine::Vector2(0,+0.3),UnityEngine::Vector2(30,8),
        [r_colorPicker]()
            {
                getMainConfig().RightSaberColor.SetValue(UnityEngine::Color(5.0, 5.0, 5.0, 5.0));
                r_colorPicker->set_currentColor(getMainConfig().RightSaberColor.GetValue());
            });    
    }

    if (!screensEnabled)
    {
        screensEnabled = true;
        mainCont->SetActive(true);
    }
}

void SaberTweaks::MainUI::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
{
    screensEnabled = false;
    mainCont->SetActive(false);
    l_saberColor->SetActive(false);
    r_saberColor->SetActive(false);
}
























// Make Hook
MAKE_HOOK_MATCH(SaberBurnMarkArea_start, &GlobalNamespace::SaberBurnMarkArea::Start, void, GlobalNamespace::SaberBurnMarkArea* self){
    SaberBurnMarkArea_start(self);
    if (getMainConfig().ModEnabled.GetValue() == true){
        if (getMainConfig().BurnLifeTime.GetValue() < 0.001f) self->set_enabled(false);
        SaberBurnMarkArea_start(self);
        float clamOpac = std::max(0.4f, getMainConfig().BurnOpacity.GetValue());
        self->burnMarksFadeOutStrength = (5.0f / clamOpac) * getMainConfig().BurnLifeTime.GetValue();
        ArrayW<UnityEngine::LineRenderer *> burnRenderers = self->lineRenderers;
        for (size_t i = 0; i < burnRenderers->Length(); i++){
            UnityEngine::LineRenderer* renderer = burnRenderers->values[i];
            static auto lr_GetStartColor = reinterpret_cast<function_ptr_t<void, UnityEngine::LineRenderer*, ByRef<UnityEngine::Color>>>(il2cpp_functions::resolve_icall("UnityEngine.LineRenderer::get_startColor_Injected"));
            UnityEngine::Color burnColor = UnityEngine::Color::get_white();
            lr_GetStartColor(renderer, burnColor);

            if (getMainConfig().OverideColors.GetValue()){
                if (i == 0) burnColor = getMainConfig().LeftSaberColor.GetValue();
                else burnColor = getMainConfig().RightSaberColor.GetValue();
            }
            renderer->get_transform()->set_localScale(UnityEngine::Vector3::get_one() * getMainConfig().BurnScale.GetValue());

            burnColor.a = clamOpac;
            renderer->set_startColor(burnColor);
            renderer->set_endColor(burnColor);
        }
    

}
}










// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    getMainConfig().Init(modInfo);
    custom_types::Register::AutoRegister();
    // QuestUI Stuff
    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController<MainUI* >(modInfo, "BurnMarkTweaks");
    QuestUI::Register::RegisterMainMenuModSettingsViewController<MainUI* >(modInfo, "Burn Mark Tweaks");
    // Custom Types
    //Install Hook:)
    INSTALL_HOOK(getLogger(), SaberBurnMarkArea_start);

    // Loggers
    getLogger().info("Installing hooks...");
    
    getLogger().info("Installed all hooks!");
}