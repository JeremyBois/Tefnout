{-# LANGUAGE OverloadedStrings #-}

-- | Module containing tools to build console command pipelines.
module ProcessTools (
  -- * Cmake
  -- $cmake
  generateBuildFiles,
  build,
  cleanAndBuild,
  install,
  lsTo,

  -- * Git
  -- $git
  initSubmodule,

  -- * Error handling
  -- $error
  assert,
  assertYes,
  assertExitSuccess,
) where

-- @INFO
-- https://stackoverflow.com/a/2141160
-- @INFO

import System.Exit (ExitCode (..))
import System.Process

import Data.Char (toUpper)

import Control.Monad.IO.Class (MonadIO, liftIO)
import Control.Monad.Trans.Except

-- | Alias to make life easier
type EIO a = ExceptT String IO a

--
-- Process / Commands
--

-- | Wrapper arround createProcess using standard stream and blocking until process is over
runAndWait :: FilePath -> [String] -> Maybe FilePath -> IO ExitCode
runAndWait cmd args workPath = do
  (_, _, _, pid) <- createProcess (proc cmd args) {cwd = workPath}
  waitForProcess pid

-- | Run cmake to generate build files using specified generator, options, and run directory.
generateBuildFiles :: String -> [String] -> FilePath -> EIO ()
generateBuildFiles generator options workingPath = do
  -- Create a new process with standard stream handle (stdin, stdout, stderr)
  code <-
    liftIO $
      runAndWait
        "cmake"
        (mconcat [["-G", generator, "-B", "./build", "-S", "."], options])
        (Just workingPath)
  assertExitSuccess
    code
    ("   |_ Target (" <> workingPath <> ") build files generation is done")
    ("   |_ Target (" <> workingPath <> ") build files generation error")

-- | Run cmake to build target defined in build files (see `generateBuildFiles`)
build :: FilePath -> EIO ()
build workingPath = do
  code <-
    liftIO $
      runAndWait "cmake" ["--build", "build"] (Just workingPath)
  assertExitSuccess
    code
    ("   |_ Target (" <> workingPath <> ") build is done")
    ("   |_ Target (" <> workingPath <> ") build error")

-- | Run cmake to first clean then build target defined in build files (see `generateBuildFiles`)
cleanAndBuild :: FilePath -> EIO ()
cleanAndBuild workingPath = do
  code <-
    liftIO $
      runAndWait "cmake" ["--build", "build", "--clean-first"] (Just workingPath)
  assertExitSuccess
    code
    ("   |_ Target (" <> workingPath <> ") clean and build is done")
    ("   |_ Target (" <> workingPath <> ") clean and build error")

-- | Run cmake to install a built target (see `build` or `cleanAndBuild`)
install :: FilePath -> EIO ()
install workingPath = do
  code <-
    liftIO $
      runAndWait "cmake" ["--install", "build"] (Just workingPath)
  assertExitSuccess
    code
    ("   |_ Target (" <> workingPath <> ") install is done")
    ("   |_ Target (" <> workingPath <> ") install error")

-- | List all files at specified directory path.
lsTo :: FilePath -> EIO ()
lsTo path = do
  (code, cout, cerr) <-
    liftIO $ readProcessWithExitCode "ls" [path] ""

  assertExitSuccess
    code
    ("   |_ Run <ls> in " <> path <> "\n" <> cout)
    ("   |_ Cannot move to " <> path <> cerr)


--
-- Git
--

-- | Init all git project submodules recursively
initSubmodule :: EIO ()
initSubmodule = do
  -- Create a new process with standard stream handle (stdin, stdout, stderr)
  code <-
    liftIO $
      runAndWait "git" ["submodule", "update", "--init", "--recursive"] Nothing
  assertExitSuccess
    code
    "   |_ Submodule initialization is done"
    "   |_ Submodule initialization error"


--
-- Assertions
--

-- | Throw and exception if predicate is False else do nothing
assert :: Bool -> String -> EIO ()
assert pred err = if pred then return () else throwE err

-- | Throw and exception if provided string is not in [yes, y, YES, Y, ...]
assertYes :: String -> EIO ()
assertYes str = assert (asUpper == "YES" || asUpper == "Y") "Aborted by user"
  where
    asUpper = fmap toUpper str

-- | Throw and exception if `ExitCode` indicate a failure.
assertExitSuccess :: ExitCode -> String -> String -> EIO ()
assertExitSuccess ExitSuccess str _ = liftIO (putStrLn str)
assertExitSuccess (ExitFailure n) _ err = throwE $ err <> " (Code=" <> show n <> ")"

--
-- Descriptions
--

{- $cmake
Wrapper of Cmake actions.
-}

{- $git
Wrapper of git actions.
-}

{- $error
Tools to handle gracefully failure in IO monad.
-}
