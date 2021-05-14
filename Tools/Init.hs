{-# LANGUAGE OverloadedStrings #-}

-- | Project initialization pipeline.
module Main (
  main,
) where

import ProcessTools

import Control.Monad.IO.Class (MonadIO, liftIO)
import Control.Monad.Trans.Except

import System.Directory
import System.IO

-- | Alias to make life easier
type EIO a = ExceptT String IO a

{- |
  Handle Tefnout project initialization automatically
    - Submodules
    - Spdlog build and install (Debug / Release) as static lib
    - GLFW build and install (Debug / Release) as Shared lib

  Currently only support make generation tool using Cmake.
-}
main :: IO (Either String ())
main = do
  at_project_root <- doesFileExist "CMakeLists.txt"
  if at_project_root
    then runExceptT actions >>= handleResult
    else liftIO $ do
      currentDir <- getCurrentDirectory
      print currentDir
      putStrLn "ERROR - This script must be run only from Tefnout root project directory."

  pure . pure $ ()

--
-- Actions
--

-- | Sequence of actions to run for initialization
actions :: EIO ()
actions = do
  rp <- liftIO $ askUser "***| Start initialization of Tefnout project ? This can take a while"
  assertYes rp

  -- LOCATE VENDORS
  liftIO $ putStrLn "***| Dependencies list"
  rootPath <- liftIO $ getCurrentDirectory >>= makeAbsolute
  let vendorsPath = rootPath <> "/Tefnout/vendors"
  lsTo vendorsPath

  -- GIT
  liftIO $ putStrLn "***| Start initialization of submodules"
  initSubmodule

  -- SPDLOG
  let spdlogLocation = vendorsPath <> "/spdlog"
  liftIO $ putStrLn $ "***| Handle spdlog initialization in " <> spdlogLocation
  spdlogActions spdlogLocation

  -- GLFW
  let glfwLocation = vendorsPath <> "/glfw"
  liftIO $ putStrLn $ "***| Handle glfw initialization in " <> glfwLocation
  glfwActions glfwLocation

  -- TEFNOUT
  liftIO $ putStrLn "***| Handle Tefnout initialization"
  tefnoutActions rootPath

-- | Generate / Build / Install libraries for spdLog
spdlogActions :: FilePath -> EIO ()
spdlogActions location = do
  mapM_
    ($ location)
    [ generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Debug",
          "-DSPDLOG_BUILD_SHARED=OFF",
          "-DSPDLOG_FMT_EXTERNAL=ON",
          "-DSPDLOG_BUILD_EXAMPLE=OFF",
          "-DSPDLOG_BUILD_TESTS=OFF",
          "-DCMAKE_POSITION_INDEPENDENT_CODE=ON",
          "-DCMAKE_INSTALL_PREFIX=./build/install/Debug"
        ],
      cleanAndBuild,
      install,
      generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Release",
          "-DSPDLOG_BUILD_SHARED=OFF",
          "-DSPDLOG_FMT_EXTERNAL=ON",
          "-DSPDLOG_BUILD_EXAMPLE=OFF",
          "-DSPDLOG_BUILD_TESTS=OFF",
          "-DCMAKE_POSITION_INDEPENDENT_CODE=ON",
          "-DCMAKE_INSTALL_PREFIX=./build/install/Release"
        ],
      cleanAndBuild,
      install
    ]

-- | Generate / Build / Install libraries for glfw
glfwActions :: FilePath -> EIO ()
glfwActions location = do
  mapM_
    ($ location)
    [ generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Debug",
          "-DBUILD_SHARED_LIBS=ON",
          "-DGLFW_BUILD_EXAMPLES=OFF",
          "-DGLFW_BUILD_TESTS=OFF",
          "-DGLFW_BUILD_DOCS=OFF",
          "-DCMAKE_INSTALL_PREFIX=./build/install/Debug"
        ],
      cleanAndBuild,
      install,
      generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Release",
          "-DBUILD_SHARED_LIBS=ON",
          "-DGLFW_BUILD_EXAMPLES=OFF",
          "-DGLFW_BUILD_TESTS=OFF",
          "-DGLFW_BUILD_DOCS=OFF",
          "-DCMAKE_INSTALL_PREFIX=./build/install/Release"
        ],
      cleanAndBuild,
      install
    ]

tefnoutActions :: FilePath -> EIO ()
tefnoutActions location = do
  mapM_
    ($ location)
    [ generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Debug",
          "-DTEFNOUT_BUILD_SHARED_LIBS=OFF",
          "-DTEFNOUT_BUILD_TESTING=ON"
        ],
      cleanAndBuild,
      generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Release",
          "-DTEFNOUT_BUILD_SHARED_LIBS=OFF",
          "-DTEFNOUT_BUILD_TESTING=ON"
        ],
      cleanAndBuild,
      generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Release",
          "-DTEFNOUT_BUILD_SHARED_LIBS=ON",
          "-DTEFNOUT_BUILD_TESTING=ON"
        ],
      cleanAndBuild,
      generateBuildFiles
        "Unix Makefiles"
        [ "-DCMAKE_BUILD_TYPE=Debug",
          "-DTEFNOUT_BUILD_SHARED_LIBS=ON",
          "-DTEFNOUT_BUILD_TESTING=ON",
          "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
        ],
      cleanAndBuild
    ]

--
-- Tools
--

askUser :: String -> IO String
askUser prompt = do
  putStr $ prompt <> " - "
  hFlush stdout -- Force flush to let user see the prompt
  answer <- liftIO getLine
  putChar '\n'
  return answer

handleResult :: Either String a -> IO ()
handleResult (Left err) =
  putStrLn $
    "\n*********************\n"
      <> "Operation aborted due to error\n"
      <> err
      <> "\n*********************\n"
handleResult (Right _) =
  putStrLn $
    "\n*********************\n"
      <> "All operations are now done, enjoy !"
      <> "\n*********************\n"
