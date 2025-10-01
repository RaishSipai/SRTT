# SpeedRush Thunder Trail (SRTT) - Comprehensive Game Design Document

## 1. Project Overview

**Title:** SpeedRush Thunder Trail (SRTT)  
**Genre:** Vehicular Racing + Combat, Battle-Royale Elements  
**Engine:** Unreal Engine 5.6 (C++ core, Blueprints, Enhanced Input)  
**Developer:** Solo indie developer  
**Platforms:** PC / Console (future mobile)  
**Style:** Semi-realistic visuals, neon accents, dynamic weather  

### Core Concept
A multiplayer vehicular combat game where players must survive by driving and shooting simultaneously. The game combines racing mechanics with combat elements in a battle royale style format with realistic manual transmission mechanics.

### Target Audience
- Players aged 12+
- Fans of high-speed racing and combat games
- Multiplayer action game enthusiasts
- Players who enjoy skill-based mechanics (manual transmission)

## 2. Core Gameplay

### Drive + Shoot Mechanics
Players simultaneously control throttle, steering, and weapon fire while managing a realistic manual transmission system.

### Manual Transmission System
- **Clutch (0–1 axis):**
  - Value < 1.0 → engine decoupled, RPM drops toward idle, gear forced to neutral
  - Only when clutch = 1.0 can you shift up/down
  - Failed shifts produce controller rumble & camera shake
  - GearboxComponent drives gear logic; vehicle physics only respond when clutch = 0

### Combat System
- **Hit-scan and projectile weapons**
- **Weapons mounted on the chassis, aiming via spring-arm camera**
- **Damage zones on vehicles (front, side, rear) for critical hits**
- **Realistic weapon handling with recoil, heat, and ammunition management**

### Vehicle Physics
- Advanced suspension simulation using Chaos Vehicle System
- Tire physics with realistic grip and wear
- Weight distribution and center of mass
- Aerodynamic forces and drag
- Realistic engine simulation with torque curves
- Transmission system with gear ratios
- Differential and drivetrain simulation
- Brake system with ABS and heat simulation
- Fuel consumption and management
- Damage system affecting vehicle performance

## 3. Game Modes

### Free-for-All (Battle Royale Race)
- Shrinking safe "blue zone"
- Red-zone bombardments outside safe zone
- Last car standing wins
- 30 players maximum
- Vehicle and weapon looting

### Team Deathmatch
- 2v2, 4v4, 6v6 team configurations
- Teams collaborate to eliminate opponents
- Objective control points
- Vehicle respawns
- Team coordination mechanics

### Duo Co-op ("Rage Gear")
- One player drives, one shoots
- Shared vehicle control
- Coordinated combat tactics
- Special duo-specific mechanics

### Solo vs. AI Bots
- Fill matches or practice mode
- Multiple difficulty levels
- AI drivers and shooters
- Mission objectives

### Objective Races
- Payload delivery
- Control-point capture
- Time-based objectives
- Team-based racing

### Time Trials with Combat
- Beat the clock while engaging bots
- Solo challenge mode
- Leaderboard integration
- Progressive difficulty

## 4. Vehicles & Classes

Each vehicle derives from `ASRTTWheeledVehiclePawn` with `UGearboxComponent` for gears & clutch.

| Class | Role | Key Stats | Weapons |
|-------|------|-----------|---------|
| Sports Car | High speed, agile | 4.5×1.9×1.3 m; mass 15 kg; 6 gears | Dual MGs, EMP |
| Muscle Car | Drift champions | 5.0×2.0×1.4 m; mass 25 kg; 5 gears | Shotgun turret, RPG |
| Supercar | Fragile speed demon | 4.3×1.8×1.2 m; mass 12 kg; 6 gears | Laser cannon, smoke dispenser |
| Off-Road Buggy | All-terrain brawler | 3.8×1.6×1.5 m; mass 18 kg; 5 gears | Grenade launcher, assault rifle |
| Monster Truck | Tanky rammer | 5.3×2.6×2.8 m; mass 75 kg; 4 gears | Rotary cannon, crush ram |
| APC | Team carrier | 5.8×2.4×2.2 m; mass 50 kg; 4 gears | Auto-cannon, flak mortar |
| Hovercraft | Land & water | 4.4×1.9×1.2 m; mass 22 kg; 8 gears | Railgun, energy shield |
| VTOL Jet | Short flight bursts | 6.0×2.5×1.8 m; mass 30 kg; 6 gears | Guided missiles, drone mines |
| Amphibious Car | Water-land hybrid | 5.0×2.2×1.7 m; mass 28 kg; 5 gears | Harpoon cannon, torpedo tube |
| Motorbike | Narrow hitbox | 2.2×0.8×1.1 m; mass 8 kg; 5 gears | Twin SMGs, smoke dispenser |
| Electric Hypercar | Silent & EMP-resistant | 4.7×2.0×1.3 m; mass 20 kg; 6 gears | Tesla coil beam, EMP burst |
| Stealth Coupe | Radar-evading | 4.2×1.8×1.2 m; mass 18 kg; 6 gears | Silenced sniper, cloaking device |

### Vehicle Classes (Traditional)
1. **Sports Cars** - High speed, low weight, precise handling, moderate armor
2. **Off-Road Vehicles** - High ground clearance, advanced suspension, heavy armor
3. **Super Cars** - Extreme speed, advanced aerodynamics, light armor
4. **Armored Vehicles** - Heavy armor, reinforced structure, multiple weapon systems

## 5. Weapons & Projectiles

### Weapon Types

#### Hitscan Weapons
- **Machine Gun** - High rate of fire, medium damage, unlimited ammo, heat-based cooldown
- **Gatling Gun** - Very high rate of fire, low damage, heat management
- **Laser** - Instant hit, high damage, energy-based

#### Projectile Weapons
- **Rocket Launcher** - High damage, limited ammo, area of effect, long reload time
- **Guided Missile** - Tracking capability, high damage
- **Grenade** - Area damage, bounce physics
- **Plasma Bolt** - Energy projectile, medium damage

#### Special Weapons
- **EMP Pulse** - Disables enemy vehicles, short duration, long cooldown, area of effect
- **Tesla Beam** - Continuous energy beam, high damage
- **Sticky Bomb** - Adheres to targets, delayed explosion

### Implementation
- `ASRTTProjectile` (mesh + UProjectileMovementComponent)
- Pawn's `FireWeapon()` spawns projectiles from `WeaponMuzzle`
- Realistic weapon handling with recoil physics, bullet ballistics, weapon heat and cooling

## 6. Power-Ups & Pickups

### Vehicle Enhancements
- **Speed Boost (nitro)** - Temporary speed increase
- **Overdrive** - 2× damage/speed for 5 seconds
- **Nano-Repair** - Regenerates health over time
- **Reactive Shield** - Absorbs incoming damage
- **Cloaking Device** - Invisibility until firing

### Additional Power-Ups
- Ammo Cache, EMP Grenade, Smoke Screen, Oil Slick, Spike Trap
- Teleport Beacon, Repair Drone, EMP Drone, Hologram Decoy, Gravity Well

### Weapon Enhancements
- Damage amplification, rate of fire increase, accuracy improvement
- Reload speed boost, special ammunition

## 7. Maps & Environments

### Seven Biomes
1. **Desert** - Open terrain, sand dunes, oasis areas
2. **Jungle** - Dense vegetation, water crossings, elevated paths
3. **Urban** - City streets, buildings, tight corners
4. **Snow** - Icy conditions, reduced traction, snowstorms
5. **Canyon** - Narrow passages, elevation changes, rock formations
6. **Volcanic** - Lava flows, unstable terrain, heat effects
7. **Underwater** - Submerged areas, pressure effects, aquatic vehicles

### Dynamic Weather
- **Rain** - Slippery conditions, reduced visibility
- **Sandstorms** - Low visibility, reduced traction
- **Snow** - Reduced traction, blizzard conditions
- **Heat waves** - Engine overheating, reduced performance

### Destructibles & Hazards
- Collapsing bridges, rolling boulders, lava flows
- Traffic AI, environmental obstacles
- Dynamic destruction system

### Zones
- **Safe Zone (Blue)** - Shrinks over time (`ASRTZoneManager`)
- **Danger Zone (Red)** - Periodic bombardments, randomized airstrikes

## 8. Input & Controls

### Enhanced Input Actions

#### Axis1D Actions
- **Throttle** - 0.0 to 1.0 acceleration
- **Brake** - 0.0 to 1.0 braking force
- **Steer** - -1.0 to 1.0 steering input
- **Clutch** - 0.0 to 1.0 clutch engagement
- **LookX** - Mouse/gamepad horizontal look
- **LookY** - Mouse/gamepad vertical look

#### Bool Actions
- **GearUp** - Shift to higher gear
- **GearDown** - Shift to lower gear
- **Fire** - Primary weapon fire
- **Handbrake** - Drift/emergency brake
- **Horn** - Vehicle horn
- **UsePowerup** - Activate power-up
- **ChangeCamera** - Switch camera views

### Control Schemes

#### Keyboard/Mouse
- **WASD** - Vehicle movement
- **Mouse** - Aim weapons
- **Left Click** - Fire
- **Right Click** - Special ability
- **Space** - Boost
- **E** - Interact/Pickup
- **Q** - Switch weapons
- **F** - Use power-up

#### Gamepad
- **Left Stick** - Vehicle movement
- **Right Stick** - Aim weapons
- **RT** - Fire
- **LT** - Special ability
- **A** - Boost
- **X** - Interact/Pickup
- **Y** - Switch weapons
- **B** - Use power-up

### Camera System
- **SpringArm + UCameraComponent** - Third-person camera
- **Look bound to AddPitchInput, AddYawInput** - Mouse/gamepad camera control
- **Multiple camera angles** - Chase, cockpit, tactical views

### Feedback Systems
- **ClientPlayForceFeedback + ClientStartCameraShake** on failed gear shifts
- **Haptic feedback** for controller users
- **Visual feedback** for all actions

## 9. UI & HUD

### HUD Classes
- **ASRTTHUD** spawns USRTTUserWidget
- **USRTTUserWidget** (Blueprint) - Speedometer, Health, Ammo, Minimap

### In-Game HUD Elements
1. **Primary Elements**
   - Health bar
   - Speedometer
   - Weapon status
   - Power-up indicators
   - Minimap
   - Gear indicator
   - Clutch position

2. **Secondary Elements**
   - Score/rank
   - Team status
   - Objective markers
   - Kill feed
   - Chat

3. **Tertiary Elements**
   - Notifications
   - Tutorial hints
   - Achievement popups
   - System messages
   - Voice chat indicators

### Menu System
- **Main Menu** - Play, Settings, Store, Profile
- **Lobby** - Match setup, team formation
- **Pause Menu** - Resume, Settings, Quit
- **Scoreboard** - Player stats, team scores
- **Settings** - Graphics, Audio, Controls, Gameplay

### Design Principles
- Clean and intuitive interface
- Consistent visual language
- Responsive feedback
- Accessibility first
- Performance optimization

## 10. Audio & VFX

### 3D Positional Audio
- **Engine Doppler effects** - Realistic engine sounds
- **Weapon SFX** - Positional weapon sounds
- **Zone warnings** - Audio cues for danger zones
- **Environmental audio** - Weather, terrain sounds

### Adaptive Music
- **Intense tracks** - Under fire, combat situations
- **Calm music** - Cruising, exploration
- **Dynamic transitions** - Based on gameplay state

### Visual Effects (Niagara)
- **Exhaust particles** - Realistic engine exhaust
- **Drift smoke** - Tire smoke effects
- **Explosions** - Weapon impact effects
- **Weather particles** - Rain, snow, sand
- **Vehicle damage** - Sparks, debris, smoke

## 11. AI & Bots

### AI Architecture
- **AIController (ASRTRobotAIController)** - AI decision making
- **Behavior Trees/Blackboards** - Complex AI behaviors
- **Navigation System** - Pathfinding and movement

### AI Roles
- **Drivers** - Focus on racing, navigation, vehicle control
- **Shooters** - Target opponents, weapon management
- **Hybrid** - Both driving and combat

### Difficulty Tiers
- **Adjustable accuracy** - Based on difficulty level
- **Aggressiveness scaling** - AI behavior modification
- **Speed adaptation** - Vehicle control difficulty
- **Team coordination** - AI teamwork capabilities

## 12. Multiplayer & Networking

### Dedicated Server Support
- **Player-hosted servers** via CLI flags
- **Epic Online Services (EOS)** integration
- **Session creation/joining** with codes
- **Cross-platform potential**

### Network Architecture
- **Client-server architecture**
- **Peer-to-peer for LAN**
- **UDP for real-time data**
- **TCP for reliable data**

### Replication System
- **Vehicles** - Transform + inputs
- **Projectiles** - Server-spawned
- **PlayerState** - Health, score, statistics
- **GameState** - Zone radius, match time, game state

### Matchmaking System
- **Skill-based algorithm**
- **Region preference**
- **Ping optimization**
- **Party support**
- **Custom match options**

### Security Measures
- **Server-side validation**
- **Client integrity checks**
- **Anti-cheat implementation**
- **Data protection and encryption**

## 13. Technical Requirements

### Engine and Tools
- **Unreal Engine 5.6**
- **Visual Studio 2022**
- **Git for version control**
- **Enhanced Input System**

### Required Modules
1. **Core Modules**
   - Core, CoreUObject, Engine
   - InputCore, EnhancedInput
   - PhysicsCore, ChaosVehicles
   - AIModule, GameplayTasks
   - NavigationSystem, UMG

2. **Networking Modules**
   - NetCore, OnlineSubsystem
   - Epic Online Services SDK

3. **Visual Effects**
   - Niagara, Chaos Physics
   - Lumen, Nanite

### System Requirements

#### Development
- Windows 10/11
- 16GB RAM minimum
- NVIDIA GTX 1060 or equivalent
- 100GB free storage
- Visual Studio 2022

#### Target Platform (Minimum)
- Windows 10
- 8GB RAM
- NVIDIA GTX 960 or equivalent
- 20GB storage
- DirectX 12 support

### Performance Targets
- 60 FPS at 1080p
- 30 FPS minimum on target hardware
- < 100ms network latency for multiplayer
- < 2GB RAM usage
- < 2s level load time

## 14. Class Architecture (Phase 1)

### Core Classes
- **USRTTGameInstance** - Game initialization and management
- **ASRTTGameModeBase** - Game rules and mode management
- **ASRTTGameStateBase** - Match state and replication
- **ASRTTPlayerState** - Player data and statistics

### Player & Input Classes
- **ASRTTPlayerController** - Input handling and player logic
- **ASRTTHUD** - HUD management and UI
- **USRTTUserWidget** - Blueprint-based UI widgets

### Vehicle Classes
- **ASRTTWheeledVehiclePawn** - Main vehicle class
- **UGearboxComponent** - Manual transmission logic

### Weapon Classes
- **ASRTTProjectile** - Projectile base class
- **Weapon firing logic** - Integrated into vehicle pawn

### Pickup Classes
- **Base ASRTPickup** - Pickup system foundation
- **Derived items** - Specific power-up implementations

## 15. Development Roadmap

### Phase 1: Core Mechanics (Months 1-2)
- [ ] Core driving mechanics with manual transmission
- [ ] Clutch and gear shifting system
- [ ] Basic firing mechanics
- [ ] HUD implementation
- [ ] TestTrack environment

### Phase 2: AI & Content (Months 3-4)
- [ ] AI bots implementation
- [ ] Additional vehicles and weapons
- [ ] Progression system
- [ ] Basic game modes

### Phase 3: Multiplayer (Months 5-6)
- [ ] Multiplayer networking
- [ ] Matchmaking system
- [ ] Dedicated server support
- [ ] Advanced game modes

### Phase 4: Content Expansion (Months 7-8)
- [ ] Additional maps and environments
- [ ] Cosmetic system
- [ ] Power-up variety
- [ ] Advanced features

### Phase 5: Polish & Launch (Months 9-10)
- [ ] Mobile port preparation
- [ ] Optimization and performance
- [ ] Live support infrastructure
- [ ] Launch preparation

## 16. Monetization Strategy

### Free-to-Play Model
- **Core game free** - No pay-to-win elements
- **Cosmetic-only purchases** - Vehicle skins, weapon skins, effects
- **Battle Pass system** - Seasonal content and rewards

### Revenue Streams
- **Vehicle skins and decals** - Visual customization
- **Weapon skins** - Weapon appearance customization
- **Special effects and animations** - Premium visual effects
- **Battle Pass** - Seasonal progression system

### Legal Requirements
- **GDPR compliance** - Data protection
- **COPPA compliance** - Child protection
- **Online transaction permissions** - Payment processing
- **Payment processing** - Stripe/PayPal integration

## 17. Publishing and Marketing

### Platforms
- **Primary** - Windows PC (Steam, Epic Games Store)
- **Future potential** - Console platforms, mobile

### Marketing Strategy
- **Steam and Epic Games Store presence**
- **Social media presence** - Community engagement
- **Regular content updates** - Seasonal content
- **Community events** - Tournaments and competitions

### Analytics and Monitoring
- **User metrics** - Session duration, feature usage
- **Performance metrics** - FPS, memory usage, network latency
- **Player behavior analysis** - Gameplay patterns and preferences

---

*This comprehensive document merges all existing documentation with the new requirements, providing a complete blueprint for the SpeedRush Thunder Trail development project.* 